/**
 * @file BasicConsole.cpp
 * @brief Source file for class BasicConsole
 * @date 05/07/2015
 * @author André Neto
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class BasicConsole (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#ifndef LINT
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#else
#include "lint-linux.h"
#endif

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicConsole.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe{

/*lint -e{9109} forward declaration in BasicConsole.h is required to define the class*/
/*lint -esym(9150, MARTe::BasicConsoleProperties::*) */
struct BasicConsoleProperties {
    /**
     * Standard output file descriptor.
     */
    static const int32 STDOUT = 1;
    /**
     * Standard input file descriptor.
     */
    static const int32 STDIN = 0;
    /**
     * Number of rows that will be cleared when BasicConsoleClear is called
     */
    static const uint32 BASIC_CONSOLE_LINUX_CLEAR_ROWS = 40u;

    /**
     * The input console handle
     */
    ConsoleHandle inputConsoleHandle;

    /**
     * The output console handle
     */
    ConsoleHandle outputConsoleHandle;

    /**
     * Initial settings of the console. The destructor will restores this initial configurations.
     */
    ConsoleHandle initialInfo;

    /**
     * The column counter keeps track of the column where the character was written to.
     */
    uint32 columnCount;

    /**
     * Flag which describes the console status.
     */
    FlagsType openingMode;

    /**
     * The number of columns currently set.
     */
    uint32 nOfColumns;

    /**
     * The number of rows currently set.
     */
    uint32 nOfRows;

};


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

BasicConsole::BasicConsole() {
    /*lint -e{1732} -e{1733} no default assignment and no default copy constructor.
     *This is safe since none of the struct members point to dynamically allocated memory*/
    handle = new BasicConsoleProperties();
    handle->columnCount = 0u;
    handle->nOfColumns = 0u;
    handle->nOfRows = 0u;
    memset(&handle->inputConsoleHandle, 0, sizeof(ConsoleHandle));
    memset(&handle->outputConsoleHandle, 0, sizeof(ConsoleHandle));
    memset(&handle->initialInfo, 0, sizeof(ConsoleHandle));
    lastPagingCounter = 0;
    lineCount = 0u;
}

BasicConsole::~BasicConsole() {
    if (handle != static_cast<BasicConsoleProperties *>(NULL)) {
        /*lint -e{534} possible closure failure is not handled in the destructor.*/
        /*lint -e{1551} exception not caught.*/
        Close();
        delete handle;
    }
}

ErrorManagement::ErrorType BasicConsole::Open(const FlagsType &mode) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;

    handle->openingMode = mode;
    //In this case read immediately from the console without wait.
    bool charactedInputSelected = (handle->openingMode & BasicConsoleMode::PerformCharacterInput) != 0u;
    if (charactedInputSelected) {
        bool ok = ioctl(fileno(stdin), static_cast<osulong>(TCGETA), &handle->outputConsoleHandle) >= 0;
        if (ok) {
            handle->initialInfo = handle->outputConsoleHandle;
            struct termio &consoleMode = handle->outputConsoleHandle;

            //use the input handle to save default parameters
            struct termio &saveMode = handle->inputConsoleHandle;
            uint16 flag = static_cast<uint16>(consoleMode.c_lflag);
            flag &= ~static_cast<uint16>(ICANON);
            consoleMode.c_lflag = flag;
            saveMode.c_cc[VMIN] = consoleMode.c_cc[VMIN];
            consoleMode.c_cc[VMIN] = 1u;
            saveMode.c_cc[VTIME] = consoleMode.c_cc[VTIME];
            consoleMode.c_cc[VTIME] = 0u;

            ok = (ioctl(fileno(stdin), static_cast<osulong>(TCSETAW), &(handle->outputConsoleHandle)) >= 0);
            if (!ok) {
                err = ErrorManagement::OSError;
                REPORT_ERROR(err,"Error: iocl()");
            }
        }
        else {
            err = ErrorManagement::OSError;
            REPORT_ERROR(err,"Error: iocl()");
        }
    }
    if (err == ErrorManagement::NoError) {
        bool ok = (fflush(stdout) == 0);
        if (!ok) {
            err = ErrorManagement::OSError;
            REPORT_ERROR(err,"Error: fflush()");
        }
    }
    return err;
}

FlagsType BasicConsole::GetOpeningMode() const {
    return handle->openingMode;
}

ErrorManagement::ErrorType BasicConsole::Close() {
    bool charactedInputSelected = (handle->openingMode & BasicConsoleMode::PerformCharacterInput) != 0u;
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (charactedInputSelected) {
        //reset the original console modes
        struct termio &consoleMode = handle->outputConsoleHandle;
        struct termio &saveMode = handle->inputConsoleHandle;
        uint16 flag = static_cast<uint16>(consoleMode.c_lflag);
        flag |= static_cast<uint16>(ICANON);
        consoleMode.c_lflag = flag;
        consoleMode.c_cc[VMIN] = saveMode.c_cc[VMIN];
        consoleMode.c_cc[VTIME] = saveMode.c_cc[VTIME];
        bool ok = (ioctl(fileno(stdin), static_cast<osulong>(TCSETAW), &handle->initialInfo) >= 0);
        if (!ok) {
            err = ErrorManagement::OSError;
            REPORT_ERROR(err,"Error: iocl()");
        }
    }
    return err;
}

/*lint -e{715} timeout is not used in Linux*/
ErrorManagement::ErrorType BasicConsole::Write(const char8 * const buffer,
                              uint32 & size,
                              const TimeoutType &timeout) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if ((handle->openingMode & BasicConsoleMode::EnablePaging) == BasicConsoleMode::EnablePaging) {
        err = PagedWrite(buffer, size, timeout);
    }
    else {
        err = OSWrite(buffer, size, timeout);
    }
    return err;
}

/*lint -e{715} timeout is not used in Linux*/
ErrorManagement::ErrorType BasicConsole::OSWrite(const char8* const buffer,
                                uint32 &size,
                                const TimeoutType &timeout) {
    const char8 *bufferString = buffer;
    const char8 newLine = '\n';

    ssize_t writtenBytes = 0;

    char8 currentChar = '\0';
    uint32 currentColumn = handle->columnCount;
    uint32 index = 0u;
    uint32 start = 0u;
    uint32 sizeToWrite = 0u;

    bool sink = false;
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    while ((err == ErrorManagement::NoError) && (index < size)) {
        currentChar = bufferString[index];
        if (currentChar == '\n') {
            //reset the currentColumn
            currentColumn = 0u;
        }

        if ((currentChar == '\n') || (index == (size - 1u))) {
            sink = true;
        }

        if (currentColumn == handle->nOfColumns) {
            sink = true;
        }

        if (sink) {
            sizeToWrite = (index - start) + 1u;

            if (sizeToWrite > 0u) {
                ssize_t wbytes = write(BasicConsoleProperties::STDOUT, &bufferString[start], static_cast<osulong>(sizeToWrite));
                if (wbytes == -1) {
                    err = ErrorManagement::OSError;
                    REPORT_ERROR(err,"Error: write()");
                }
                writtenBytes += wbytes;
            }
            start = index + 1u;
            sink = false;
        }
        if (currentColumn == handle->nOfColumns) {
            ssize_t wbytes = write(BasicConsoleProperties::STDOUT, &newLine, static_cast<osulong>(1));
            if (wbytes == -1) {
                err = ErrorManagement::OSError;
                REPORT_ERROR(err,"Error: write()");
            }
            currentColumn = 0u;
        }

        currentColumn++;
        index++;
    }
    handle->columnCount = currentColumn;

    size = static_cast<uint32>(writtenBytes);
    if (size == 0u) {
        err = ErrorManagement::Warning;
        REPORT_ERROR(err,"ErrorManagement::Warning: zero bytes written");
    }
    return err;
}

/*lint -e{715} timeout is not used...*/
ErrorManagement::ErrorType BasicConsole::Read(char8 * const buffer,
                             uint32 & size,
                             const TimeoutType &timeout) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if ((buffer != NULL) && (size > 0u)) {
        if ((handle->openingMode & BasicConsoleMode::PerformCharacterInput) != 0u) {
            char8 *readChar = buffer;
            *readChar = static_cast<char8>(getchar());
            int32 eofCheck = static_cast<int32>(*readChar);
            if (eofCheck == EOF) {
                err = ErrorManagement::OSError;
                REPORT_ERROR(err,"Error: getchar()");
            }
            else {
                size = 1u;
            }
        }
        else {
            ssize_t readBytes = read(BasicConsoleProperties::STDIN, buffer, static_cast<osulong>(size));
            if (readBytes == -1) {
                err = ErrorManagement::OSError;
                REPORT_ERROR(err,"Error: read()");
            }
            else if (readBytes == 0) {
                err = ErrorManagement::Warning;
                REPORT_ERROR(err,"ErrorManagement::Warning: zero bytes read");
            }
            else {
                size = static_cast<uint32>(readBytes);
            }
        }
    }
    else {
        err = ErrorManagement::Warning;
        REPORT_ERROR(err,"ErrorManagement::Warning: invalid input parameters");
    }
    return err;
}

ErrorManagement::ErrorType BasicConsole::SetSize(const uint32 &numberOfColumns,
                                const uint32 &numberOfRows) {
    handle->nOfColumns = numberOfColumns;
    handle->nOfRows = numberOfRows;
    return ErrorManagement::NoError;
}

ErrorManagement::ErrorType BasicConsole::GetSize(uint32 &numberOfColumns,
                                uint32 &numberOfRows) const {
    numberOfColumns = handle->nOfColumns;
    numberOfRows = handle->nOfRows;
    return ErrorManagement::NoError;
}

ErrorManagement::ErrorType BasicConsole::Clear() {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    for (uint32 i = 0u; i < BasicConsoleProperties::BASIC_CONSOLE_LINUX_CLEAR_ROWS; i++) {
        ssize_t writtenBytes = write(BasicConsoleProperties::STDOUT, "\n", static_cast<osulong>(1u));
        if (writtenBytes == -1) {
            err = ErrorManagement::OSError;
            REPORT_ERROR(err,"Error: write()");
        }
    }
    return err;
}

bool BasicConsole::CursorPositionSupported() const {
    return false;
}

bool BasicConsole::TitleBarSupported() const {
    return false;
}

bool BasicConsole::ColourSupported() const {
    return false;
}

bool BasicConsole::WindowSizeSupported() const {
    return false;
}

bool BasicConsole::ConsoleBufferSupported() const {
    return false;
}

bool BasicConsole::TimeoutSupported() const {
    return false;
}

/*lint -e{715} function not implemented in Linux*/
ErrorManagement::ErrorType BasicConsole::ShowBuffer() {
    REPORT_ERROR(ErrorManagement::UnsupportedFeature,"Information: function not implemented");
    return ErrorManagement::UnsupportedFeature;
}

/*lint -e{715} function not implemented in Linux*/
ErrorManagement::ErrorType BasicConsole::SetColour(const Colours &foregroundColour,
                                  const Colours &backgroundColour) {
    REPORT_ERROR(ErrorManagement::UnsupportedFeature,"Information: function not implemented");
    return ErrorManagement::UnsupportedFeature;
}

/*lint -e{715} function not implemented in Linux*/
ErrorManagement::ErrorType BasicConsole::SetTitleBar(const char8 * const title) {
    REPORT_ERROR(ErrorManagement::UnsupportedFeature,"Information: function not implemented");
    return ErrorManagement::UnsupportedFeature;
}

/*lint -e{715} function not implemented in Linux*/
ErrorManagement::ErrorType BasicConsole::GetTitleBar(char8 * const title,
                                    const uint32 &size) const {
    REPORT_ERROR(ErrorManagement::UnsupportedFeature,"Information: function not implemented");
    return ErrorManagement::UnsupportedFeature;
}

/*lint -e{715} function not implemented in Linux*/
ErrorManagement::ErrorType BasicConsole::SetCursorPosition(const uint32 &column,
                                          const uint32 &row) {
    REPORT_ERROR(ErrorManagement::UnsupportedFeature,"Information: function not implemented");

    return ErrorManagement::UnsupportedFeature;
}

/*lint -e{715} function not implemented in Linux*/
ErrorManagement::ErrorType BasicConsole::GetCursorPosition(uint32 &column,
                                          uint32 &row) const {
    return ErrorManagement::UnsupportedFeature;
}

/*lint -e{715} function not implemented in Linux*/
ErrorManagement::ErrorType BasicConsole::SetWindowSize(const uint32 &numberOfColumns,
                                      const uint32 &numberOfRows) {
    REPORT_ERROR(ErrorManagement::UnsupportedFeature,"Information: function not implemented");

    return ErrorManagement::UnsupportedFeature;
}

/*lint -e{715} function not implemented in Linux*/
ErrorManagement::ErrorType BasicConsole::GetWindowSize(uint32 &numberOfColumns,
                                      uint32 &numberOfRows) const {
    REPORT_ERROR(ErrorManagement::UnsupportedFeature,"Information: function not implemented");

    return ErrorManagement::UnsupportedFeature;
}

/*lint -e{715} function not implemented in Linux*/
ErrorManagement::ErrorType BasicConsole::PlotChar(const char8 &c,
                                 const Colours &foregroundColour,
                                 const Colours &backgroundColour,
                                 const uint32 &column,
                                 const uint32 &row) {
    REPORT_ERROR(ErrorManagement::UnsupportedFeature,"Information: function not implemented");

    return ErrorManagement::UnsupportedFeature;
}

}

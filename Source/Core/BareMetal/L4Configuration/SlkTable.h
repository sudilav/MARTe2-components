
// SlkTable.h - generated by the SLK parser generator 

#ifndef _SlkTABLE_H
#define _SlkTABLE_H

public:

void    End ( void );
void    GetNodeName ( void );
void    AddLeaf ( void );
void    GetTypeCast ( void );
void    CreateNode ( void );
void    AddScalar ( void );
void    EndVector ( void );
void    EndMatrix ( void );
void    BlockEnd ( void );

void    ( SlkAction::*Action [ 10 ] ) ( void );

void    initialize_table ( void );
void
execute ( int  number )   { ( this->*Action [ number ] ) (); }

#endif

#!/bin/bash
HOST=flexelint.codac.local
REMOTE_DIR_CODE=MARTe2-dev
REMOTE_DIR_DOC=MARTe2-doc
rsync -avz --delete -e ssh . $USER@$HOST:$REMOTE_DIR_CODE
ssh $USER@$HOST "cd $REMOTE_DIR_DOC && git pull"
ssh $USER@$HOST "/opt/FlexeLint/flint -i $REMOTE_DIR_CODE/Source/Core/L0Portability/ -i /opt/FlexeLint/supp/lnt -i $REMOTE_DIR_DOC/Assets/Configuration/ -v marte_flint_eclipse.lnt"

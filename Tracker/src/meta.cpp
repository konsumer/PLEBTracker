#include "patternedtr.h"
#include <iostream>

void patternedtr::chgSelMetaObj(int i)
{
    selobjmeta += i;
    switch(selrowmeta)
    {
        case 0://First row [N S O NAME TRACKS ROWS RES/DIV ? X]
            if(selobjmeta > META_ROW0_DEPTH)
                if(i>0)
                    selobjmeta = META_ROW0_DEPTH;
                else
                    selobjmeta=0;
            break;
        case 1://Second row [INSTR N C X SEL STEP OCT BEAT]
            if(selobjmeta > META_ROW1_DEPTH)
                if(i>0)
                    selobjmeta = META_ROW1_DEPTH;
                else
                    selobjmeta = 0;
            break;
        case 2: //Third row [ORDER PATTERN v ^ N C X v ^ N C Q X] ... KEY ^ v SCALE
            //if(selobjmeta > META_ROW2_DEPTH + editor::song->numOrders())
            //    if(i > 0)
            //        selobjmeta = META_ROW2_DEPTH + editor::song->numOrders()-1;
            //    else
            //        selobjmeta = 0;
            if(selobjmeta > META_ROW2_DEPTH)
                if(i > 0)
                    selobjmeta = META_ROW2_DEPTH;
                else
                    selobjmeta = 0;
            break;
    }
}

void patternedtr::chgSelMetaRow(int i)
{
    selrowmeta += i;
    if(selrowmeta >= META_ROWS)
        if(i > 0)
            selrowmeta = META_ROWS-1;
        else
            selrowmeta = 0;
    else
    {
        //Could be nice to revise this later to make more sense
        if(selrowmeta == 0)
            selobjmeta = 1;
        else if(selrowmeta == 1)
            selobjmeta = 4;
        else if(selrowmeta == 2)
            selobjmeta = 0;
    }
}

void patternedtr::getKeyChar(char *bfr, unsigned char note)
{
    bfr[2] = 0;
    if(note == 0xFF)
    {
        bfr[0] = '_';
        bfr[1] = '_';
        return;
    }
    bfr[1] = ' ';

    switch(note)
    {
        case 0:
            bfr[0] = 'C';
            break;
        case 1:
            bfr[0] = 'C';
            bfr[1] = '#';
            break;

        case 2:
            bfr[0] = 'D';
            break;
        case 3:
            bfr[0] = 'D';
            bfr[1] = '#';
            break;

        case 4:
            bfr[0] = 'E';
            break;

        case 5:
            bfr[0] = 'F';
            break;
        case 6:
            bfr[0] = 'F';
            bfr[1] = '#';
            break;

        case 7:
            bfr[0] = 'G';
            break;
        case 8:
            bfr[0] = 'G';
            bfr[1] = '#';
            break;

        case 9:
            bfr[0] = 'A';
            break;
        case 0xA:
            bfr[0] = 'A';
            bfr[1] = '#';
            break;

        case 0xB:
            bfr[0] = 'B';
            break;
    }

}


void patternedtr::getScale_Sel(char *bfr, unsigned char *scale)
{
    int acc = 0;
    int i;
    

    for(i = 0; i < CHROMATIC_NOTES-1 && acc < CHROMATIC_NOTES; i++)
    {
        acc += scale[i];
        bfr[i] = ('0' + scale[i]);
    }

    scale[i] = 0;

    for(i; i < CHROMATIC_NOTES-1; i++)
        bfr[i] = ' ';
    bfr[CHROMATIC_NOTES-1] = 0;
}

void patternedtr::getScale_Unsel(char *bfr, unsigned char scalespnr, unsigned char *scale)
{
    int acc = 0;
    for(int i = 0; i < CHROMATIC_NOTES-1; i++)
        bfr[i] = ' ';
    switch(scalespnr)
    {
        case 0: 
            strcpy(bfr, "Chromatic");
            bfr[9]=' ';
            break;

        case 1: 
            strcpy(bfr, "Major");
            bfr[5]=' '; //set the copied nullchar to space
            break;
        case 2: 
            strcpy(bfr, "Minor");
            bfr[5]=' ';
            break;
        case 3: 
            strcpy(bfr, "Minor Hrmon");
            break;
        case 4: 
            strcpy(bfr, "Minor Melod");
            break;

        case 5: 
            strcpy(bfr, "Dorian");
            bfr[6]=' ';
            break;
        case 6: 
            strcpy(bfr, "Phyrgian");
            bfr[8]=' ';
            break;
        case 7: 
            strcpy(bfr, "Lydian");
            bfr[6]=' ';
            break;
        case 8: 
            strcpy(bfr, "Mixolydian");
            bfr[10]=' ';
            break;
        case 9: 
            strcpy(bfr, "Locrian");
            bfr[7]=' ';
            break;

        default:
            getScale_Sel(bfr, scale);
    }
    bfr[CHROMATIC_NOTES-1] = 0;
}



unsigned char patternedtr::inferScaleType(unsigned char *scale)
{
    //Make an alias as signed for strcmp
    char * scaleascii = (char *)scale;
    unsigned char out = 0xFF;
    //convert to ascii in order to use strcmp
    for(int i = 0; i < CHROMATIC_NOTES-1; i++)
    {
        if(scale[i] != 0)
            scale[i] += '0';
    }

    using namespace editor;
    //It's just easier this way, unless I made a cmp function for scale
    if(strcmp(scaleascii, "11111111111") == 0)
        out = SCALE_CHROMATIC;
    else if(strcmp(scaleascii, "2212221") == 0) //MAJOR
        out = SCALE_MAJOR;
    else if(strcmp(scaleascii, "2122122") == 0) //MINOR (AEOLIAN)
        out = SCALE_AEOLIAN;
    else if(strcmp(scaleascii, "2122131") == 0) //MINOR (HARMONIC)
        out = SCALE_HARMONIC;
    else if(strcmp(scaleascii, "2122221") == 0) //MINOR (MELODIC)
        out = SCALE_MELODIC;
    else if(strcmp(scaleascii, "2122212") == 0) //DORIAN
        out = SCALE_DORIAN;
    else if(strcmp(scaleascii, "1222122") == 0) //PHYRGIAN
        out = SCALE_PHYRGIAN;
    else if(strcmp(scaleascii, "2221221") == 0) //LYDIAN
        out = SCALE_LYDIAN;
    else if(strcmp(scaleascii, "2212212") == 0) //MIXOLYDIAN
        out = SCALE_MIXOLYDIAN;
    else if(strcmp(scaleascii, "1221222") == 0) //LOCRIAN
        out = SCALE_LOCRIAN;

    for(int i = 0; i < CHROMATIC_NOTES-1; i++)
    {
        if(scale[i] != 0)
            scale[i] -= '0';
    }

    return out;
}


bool patternedtr::generateScaleFromType(unsigned char *scale, unsigned char scalespnr)
{

    using namespace editor;
    switch(scalespnr)
    {
        case SCALE_CHROMATIC:
            for(int i = 0; i < CHROMATIC_NOTES-1; i++)
                scale[i]='1';
            break;
        case SCALE_MAJOR:
            strcpy((char*)scale, "2212221");
            break;
        case SCALE_AEOLIAN:
            strcpy((char*)scale, "2122122");
            break;
        case SCALE_HARMONIC:
            strcpy((char*)scale, "2122131");
            break;
        case SCALE_MELODIC:
            strcpy((char*)scale, "2122221");
            break;
        case SCALE_DORIAN:
            strcpy((char*)scale, "2122212");
            break;
        case SCALE_PHYRGIAN:
            strcpy((char*)scale, "1222122");
            break;
        case SCALE_LYDIAN:
            strcpy((char*)scale, "2221221");
            break;
        case SCALE_MIXOLYDIAN:
            strcpy((char*)scale, "2212212");
            break;
        case SCALE_LOCRIAN:
            strcpy((char*)scale, "1221222");
            break;
        default:
            return false;
    }
    int i;
    for(i = 0; i < CHROMATIC_NOTES && scale[i] != 0; i++)
        scale[i] -= '0';
    for(; i < CHROMATIC_NOTES; i++)
        scale[i] = 0;
    return true;

}


bool patternedtr::setMetaAttribs(unsigned char objmeta, unsigned char rowmeta)
{
    attroff(-1);
    using namespace patternedtr;
    if(editor::inputwin == editor::metawin)
    {
        //Window selected
        attron(A_BOLD);
        if(objmeta == selobjmeta && rowmeta == selrowmeta)
        {
            //Object and Row Selected
            if(metaobjedit)
                attron(COLOR_PAIR(COL_META_SSSE)); //Is editing this obj
            else
                attron(COLOR_PAIR(COL_META_SSS));  //Selected
            return true;
        }
        else
            attron(COLOR_PAIR(COL_META_SU)); //Is not selected
    }
    else
    {
        //Window not selected
        if(objmeta == selobjmeta && rowmeta == selrowmeta)
            attron(COLOR_PAIR(COL_META_US));//But obj focussed
        else
            attron(COLOR_PAIR(COL_META_UU));//just unselected
    }
    return false;
}



char *editor::makeUnderlines(char *string, int length)
{
    //Fill string the underlines
    for(int i = 0; i < length; i++)
    {
        if(string[i] == 0)
            string[i] = '_';
    }
    return string;
}



void patternedtr::displayMeta()
{
    using namespace editor;
    const bool WINCHANGE = lastwin != metawin;
    if(WINCHANGE)
    {
        lastwin = metawin;
        //using a buffer will be far more efficient than setting each individual cell
        //and for some reason im adversed to using dynamic memory here so yeah
        

        /////////////////////////////////////////////////////////////////
        //CLEAR THE META WINDOW/////to clean up potential artifacts/////
        //clear the majority of the meta region
        int div = editor::WIN_WIDTH / 12;
        int rem = editor::WIN_WIDTH % 12;
        char bfr[12];
        int i;
        for(i = 0; i < div; i++)
            for(int j = 0; j < 3; j++)
                mvprintw(j,12*i, "            ", editor::metawin);//12 spaces

        //fill buffer to size of remainder
        for(i = 0; i < rem; i++)
            bfr[i] = ' ';
        bfr[i] = 0;

        //fill in remaining spaces
        for(i = 0; i < 3; i++)
            mvprintw(i, editor::WIN_WIDTH-(rem), bfr, editor::metawin);
        //DONE CLEARING///////////////////////////////////////////

    }

    bool isselected;
    //Disable attributes
    attroff(-1);

    //Print Top Bar, labels
    //Row 0
    mvprintw(0, 0, "[NSO] ", metawin);
    mvprintw(0, 35, " TRACKS:", metawin);
    mvprintw(0, 46, " ROWS:", metawin);
    mvprintw(0, 55, " BYT/SEG:____/__", metawin);
    mvprintw(0, 72, "   [? X]", metawin);

    //New
    setMetaAttribs(0,0);
    mvprintw(0,1,"N",metawin);
    //Save
    setMetaAttribs(1,0);
    mvprintw(0,2,"S",metawin);
    //Open
    setMetaAttribs(2,0);
    mvprintw(0,3,"0",metawin);


    //Song title
    isselected = setMetaAttribs(3, 0);
    if(isselected && metaobjedit)
    {
        copy(charInputBuffer, charBuffer, SONGNAME_LENGTH);
        makeUnderlines(charBuffer, SONGNAME_LENGTH);
        mvprintw(0, 6, charBuffer, metawin);
        attron(COLOR_PAIR(COL_META_UU));
        char sel[2];
        sel[0] = charBuffer[textCursorPos];
        sel[1] = 0;
        mvprintw(0, 6 + textCursorPos, sel, metawin);
    }
    else
    {
        charBuffer[SONGNAME_LENGTH+1]=0;
        copy(editor::song->getName(), charBuffer, SONGNAME_LENGTH);
        makeUnderlines(charBuffer, SONGNAME_LENGTH);
        mvprintw(0, 6, charBuffer, metawin);
    }

    //Tracks
    charBuffer[2] = 0;
    isselected = setMetaAttribs(4, 0);
    if(isselected && metaobjedit)
        byteString(charBuffer, numBuffer);
    else
        byteString(charBuffer, editor::song->numTracks());
    mvprintw(0, 43, charBuffer, metawin);

    //Rows
    isselected = setMetaAttribs(5, 0);
    if(isselected && metaobjedit)
        byteString(charBuffer, numBuffer);
    else
        byteString(charBuffer, patternedtr::selptrn->numRows());
    mvprintw(0, 52, charBuffer, metawin);

    //Resolution
    charBuffer[4] = 0;
    isselected = setMetaAttribs(6, 0);
    if(isselected && metaobjedit)
        shortString(charBuffer, numBuffer, 4);
    else
        shortString(charBuffer, editor::song->getBytesPerRow(), 4);

    if(!metaobjedit && (editor::song->getBytesPerRow() % editor::song->getInterRowResolution()) > 0)
    {
        if(isselected)
            attron(COLOR_PAIR(COL_META_SSS_ERR));
        else
            attron(COLOR_PAIR(COL_META_ERR));
    }
    mvprintw(0, 64, charBuffer, metawin);

    //Div
    charBuffer[2] = 0;
    isselected = setMetaAttribs(7,0);
    if(isselected && metaobjedit)
        byteString(charBuffer, numBuffer);
    else
    {
        
        byteString(charBuffer, editor::song->getInterRowResolution());
    }

    if(!metaobjedit && (editor::song->getBytesPerRow() % editor::song->getInterRowResolution()) > 0)
    {
        if(isselected)
            attron(COLOR_PAIR(patternedtr::COL_META_SSS_ERR));
        else
            attron(COLOR_PAIR(patternedtr::COL_META_ERR));
    }

    mvprintw(0, 69, charBuffer, metawin);


    //About
    setMetaAttribs(8,0);
    mvprintw(0,76,"?",metawin);

    //Exit
    setMetaAttribs(9,0);
    mvprintw(0,78,"X",metawin);

    //////////////////////////////////////////////
    //////////////////////////////////////////////
    //ROW 1       ////////////////////////////////
    //////////////////////////////////////////////
    //////////////////////////////////////////////


    //Print labels
    attroff(-1);
    mvprintw(1, 0, "INSTR ", metawin);
    mvprintw(1, 29, " [NCX] ", metawin);
    //print order //two char hex
    mvprintw(1, 38," STEP:", metawin);
    mvprintw(1, 45, "  OCT :", metawin);
    mvprintw(1, 53, "   BEAT:_", metawin);
    //print octave //one char hex

    //Instrument name
    isselected = setMetaAttribs(0, 1);
    copy(editor::selinst->getName(), charBuffer, INST_NAME_SIZE);
    charBuffer[INST_NAME_SIZE-1] = 0;
    mvprintw(1, 6, makeUnderlines(charBuffer,INST_NAME_SIZE-1), metawin); 

    charBuffer[2] = 0;
    isselected = setMetaAttribs(4, 1);
    if(isselected && metaobjedit)
        byteString(charBuffer, numBuffer);
    else
        byteString(charBuffer, patternedtr::selinstrument);
    mvprintw(1, 36, charBuffer);

    //New Instrument
    setMetaAttribs(1,1);
    mvprintw(1,31,"N",metawin);

    //Clone Instrument
    setMetaAttribs(2,1);
    mvprintw(1,32,"C",metawin);
    
    //Remove Instrument
    setMetaAttribs(3,1);
    mvprintw(1,33,"X",metawin);

    //Edit Step
    isselected = setMetaAttribs(5, 1);
    if(isselected && metaobjedit)
        byteString(charBuffer, numBuffer);
    else
        byteString(charBuffer, patternedtr::edit_step);
    mvprintw(1, 44, charBuffer+1);

    //Octave
    isselected = setMetaAttribs(6, 1);
    if(isselected && metaobjedit)
        byteString(charBuffer, numBuffer);
    else
        editor::byteString(charBuffer, patternedtr::octave);
    mvprintw(1, 52, charBuffer+1);


    //Beat/Row lines
    isselected = setMetaAttribs(7, 1);
    if(isselected && metaobjedit)
    {
        byteString(charBuffer, numBuffer);
        mvprintw(1, 61, charBuffer+1);
    }
    else
    {
        if(patternedtr::row_underline > 0)
        {
            editor::byteString(charBuffer, patternedtr::row_underline);
            mvprintw(1, 61, charBuffer+1);
        }
        else if(isselected)
            mvprintw(1, 61, "_");
    }

    //////////////////////////////////////////////
    //////////////////////////////////////////////
    //ROW 2       ////////////////////////////////
    //////////////////////////////////////////////
    //////////////////////////////////////////////

    attroff(-1);
    mvprintw(2, 0, "ORDER [  -  ] [v^ NCX v^ NCWX]                 KEY :__  [v^]", metawin);

    //Order
    isselected = setMetaAttribs(0,2);
    if(isselected && metaobjedit)
        byteString(charBuffer, numBuffer);
    else
        byteString(charBuffer, patternedtr::selorder);
    mvprintw(2,7, charBuffer, metawin);

    //Pattern
    isselected = setMetaAttribs(1,2);
    if(isselected && metaobjedit)
        byteString(charBuffer, numBuffer);
    else
        byteString(charBuffer, song->getPatternIndexByOrder(selorder));
    mvprintw(2,10, charBuffer, metawin);

    //Down order
    setMetaAttribs(2,2);
    mvprintw(2,15,"v",metawin);
    //Up order
    setMetaAttribs(3,2);
    mvprintw(2,16,"^",metawin);

    //New Order
    setMetaAttribs(4,2);
    mvprintw(2,18,"N",metawin);
    //Clone Order
    setMetaAttribs(5,2);
    mvprintw(2,19,"C",metawin);
    //Remove Order
    setMetaAttribs(6,2);
    mvprintw(2,20,"X",metawin);


    //Down Pattern
    setMetaAttribs(7,2);
    mvprintw(2,22,"v",metawin);
    //Up Pattern
    setMetaAttribs(8,2);
    mvprintw(2,23,"^",metawin);
    //New Pattern
    setMetaAttribs(9,2);
    mvprintw(2,25,"N",metawin);
    //Clone Pattern
    setMetaAttribs(10,2);
    mvprintw(2,26,"C",metawin);
    //Clear Pattern
    setMetaAttribs(11,2);
    mvprintw(2,27,"Q",metawin);
    //Remove Pattern
    setMetaAttribs(12,2);
    mvprintw(2,28,"X",metawin);

    //Key
    isselected = setMetaAttribs(13, 2);
    if(isselected && metaobjedit)
    {
        getKeyChar(charBuffer, numBuffer);
        mvprintw(2, 60-8, charBuffer);
    }
    else
        if(patternedtr::key != 0xFF || isselected)
        {
            getKeyChar(charBuffer, patternedtr::key);
            mvprintw(2, 60-8, charBuffer);
        }
    //Up Key
    setMetaAttribs(14, 2);
    mvprintw(2,64-7,"v",metawin);

    //Down Key
    setMetaAttribs(15, 2);
    mvprintw(2,65-7,"^",metawin);

    //Print Scale
    isselected = setMetaAttribs(16, 2);
    if(isselected && metaobjedit)
    {
        getScale_Sel(charBuffer, patternedtr::scaleconst);
        mvprintw(2,68-7, charBuffer, metawin);

        attron(COLOR_PAIR(COL_META_UU));
        char sel[2];
        sel[0] = charBuffer[textCursorPos];
        sel[1] = 0;
        mvprintw(2, 68 -7+ textCursorPos, sel, metawin);
    }
    else
    {
        getScale_Unsel(charBuffer, patternedtr::scalespinner, patternedtr::scaleconst);
        mvprintw(2,68-7, charBuffer, metawin);

    }

    //Print metaobjedit information
    if(metaobjedit)
    {
        attron(A_BOLD);
        mvprintw(WIN_HEIGHT-1, 0, "Editing a field: Press TAB to cancel or ENTER to commit", stdscr);
        attroff(A_BOLD);
    }

}
void patternedtr::startMetaEditing()
{

    bool confirmation;
    ///////////////////////
    /////////    BUTTONS///
    ///////////////////////
    if(selrowmeta == 0)
    {
        switch(selobjmeta)
        {
            case 0:
                //New Song ||Button [0,0]

                confirmation = editor::confirm("This will delete any unsaved data");
                if(confirmation)
                    newSong();
                return;
            case 1:
                //Save Song ||Button [0,1]

                confirmation = editor::confirm("Save?");
                if(confirmation)
                {
                    //const char *file = browse();
                    const char *file = editor::lastSongPath;
                    if(file)
                        saveSong(file);
                }

                return;
            case 2:
                //Open Song ||Button [0,2]

                /*
                confirmation = editor::confirm("Open? Any unsaved data will be lost");
                if(confirmation)
                {
                    //const char *file = browse();
                    const char *file = editor::lastSongPath;
                    if(file)
                        openSong(file);
                }*/

                if(editor::getCommand("o "))
                    editor::doCommand();

                if(editor::wingroup == editor::ptrnwin)
                    patternedtr::display();
                else if(editor::wingroup == editor::instwin)
                    instedtr::display();

                return;
            case 8:
                //About ||Button [0,7]
                editor::displayAbout();
                return;
            case 9:
                //Exit ||Button [0,8]
                patternedtr::exit();
                return;
        }
    }
    else if(selrowmeta == 1)
    {
        switch(selobjmeta)
        {
            case 0:
                //Instrument Edit ||Big Button [1,0]
                editor::inputwin = editor::instwin;
                editor::wingroup = editor::instwin;
                return;
            case 1:
                //New Instrument ||Button [1,1]
               newInstrument();
                return;
            case 2:
                //Clone Instrument ||Button [1,2]
               cloneInstrument(patternedtr::selinstrument);
                return;
            case 3:
                //Remove Instrument ||Button [1,3]
                removeInstrument(patternedtr::selinstrument);                
                return;
        }
    }
    else if(selrowmeta == 2)
    {
        switch(selobjmeta)
        {
            case 2:
                //DOWN Order ||Button
               chgSelOrder(-1);
                return;
            case 3:
                //UP Order ||Button
                chgSelOrder(1);
                return;
            case 4:
                //New Order ||Button
                //Create a new order with a new pattern
               newOrder(); 
                return;
            case 5:
                //Clone Order ||Button
               cloneOrder(patternedtr::selorder);
                return;
            case 6:
                //Remove Order ||Button
               removeOrder(patternedtr::selorder);
                return;
            case 7:
                //DOWN Pattern ||Button [2,2]
                chgOrderPattern(-1);
                return;
            case 8:
                //UP Pattern ||Button [2,3]
                chgOrderPattern(1);
                return;
            case 9:
                //New Pattern ||Button [2,4]
                newPattern();
                return;
            case 10:
                //Clone Pattern ||Button [2,5]
               clonePattern(editor::song->getPatternIndexByOrder(selorder));
                return;
            case 11:
                //Clear Pattern ||Button [2,6]
                clearPattern(editor::song->getPatternIndexByOrder(selorder));
                return;
            case 12:
                //Remove Pattern ||Button [2,7]
               removePattern(editor::song->getPatternIndexByOrder(selorder)); 
                return;
            case 14:
                //Cycle Scale Up
                if(scalespinner < 9)
                    scalespinner++;
                else scalespinner = 0;

                generateScaleFromType(scaleconst, scalespinner);
                populateNoteMap();
                return;
            case 15:
                //Cycle Scale Down
                if(scalespinner > 0)
                    scalespinner--;
                else scalespinner = 9;

                generateScaleFromType(scaleconst, scalespinner);
                populateNoteMap();
                return;
        }
    }

    ///////////////////////////////////////////
    /////////  EDITABLE FIELDS / NON-BUTTONS///
    ///////////////////////////////////////////

    //Depending on what you're editing, set up the buffers
    if(selrowmeta == 0)
    {
        //Song Name
        if(selobjmeta ==3)
        {
            editor::copy(editor::song->getName(), editor::charInputBuffer, SONGNAME_LENGTH);

            editor::textCursorPos = strlen(editor::song->getName());
            if(editor::textCursorPos > SONGNAME_LENGTH-1)
                editor::textCursorPos = SONGNAME_LENGTH-1;

            metaobjedit = true;
        }
        //Tracks
        else if(selobjmeta == 4)
        {
            editor::numBuffer = 0;
            metaobjedit = true;
        }
        //Rows
        else if(selobjmeta == 5)
        {
            editor::numBuffer = 0;
            metaobjedit = true;
        }
        //BPR
        else if(selobjmeta == 6)
        {
            editor::numBuffer = editor::song->getBytesPerRow();
            metaobjedit = true;
        }
        //DIV
        else if(selobjmeta == 7)
        {
            editor::numBuffer = editor::song->getInterRowResolution();
            metaobjedit = true;
        }
    }
    else if(selrowmeta == 1) 
    {
        //Selected instrument spinner
        if(selobjmeta == 4)
        {
            editor::numBuffer = selinstrument;
            metaobjedit = true;
        }
        //Edit step spinner
        else if(selobjmeta == 5)
        {
            editor::numBuffer = edit_step;
            metaobjedit = true;
        }
        //Octave spinner
        else if(selobjmeta == 6)
        {
            editor::numBuffer = octave;
            metaobjedit = true;
        }
        
        //Rows per beat underline spinner
        else if(selobjmeta == 7)
        {
            editor::numBuffer = row_underline;
            metaobjedit = true;
        }
    }
    else if(selrowmeta == 2)
    {
        //Selected order spinner
        if(selobjmeta == 0)
        {
            editor::numBuffer = selorder;
            metaobjedit = true;
        }
        //Order's Pattern spinner
        else if(selobjmeta == 1)
        {
            editor::numBuffer = editor::song->getPatternIndexByOrder(selorder);
            metaobjedit = true;
        }
        //Key custom spinner
        else if(selobjmeta == 13)
        {
            editor::numBuffer = key;
            metaobjedit = true;
        }
        else if(selobjmeta == 16)
        {
            editor::textCursorPos = 0;
            metaobjedit = true;
        }
    }

}

void patternedtr::metaEdit(wint_t in)
{
    using namespace editor;
    bool ishex = editor::validateHexChar(in);
    bool isarrow = (in == KEY_LEFT||in==KEY_RIGHT||in==KEY_UP||in==KEY_DOWN);
    bool spinnervalid = ishex || isarrow;
    char hexnum = charHex(in);

    if(selrowmeta == 0)
    {
        switch(selobjmeta)
        {
            case 3:
                //Name ||Textbox [0,3]
                if(isarrow)
                {
                    if(in == KEY_LEFT)
                    {
                        if(textCursorPos > 0)
                            textCursorPos--;
                    }
                    else if(in == KEY_RIGHT)
                    {
                        int length = strlen(charInputBuffer);

                        if(textCursorPos < length)
                            if(textCursorPos < SONGNAME_LENGTH-1)
                                textCursorPos++;
                    }
                }
                else if(in == ALT_BACKSPACE)
                {
                    if(textCursorPos > 0)
                    {
                        int length = SONGNAME_LENGTH;
                        for(int i = textCursorPos; i < length+1; i++)
                            charInputBuffer[i-1] = charInputBuffer[i];
                        textCursorPos--;
                    }

                }
                else if(in == KEY_DC)
                {
                    int length = strlen(charInputBuffer);
                    for(int i = textCursorPos+1; i < length+1; i++)
                        charInputBuffer[i-1] = charInputBuffer[i];
                    if(textCursorPos >= length-1 && textCursorPos > 0)
                        textCursorPos--;

                }
                else if(in == KEY_IC)
                {
                    int length = strlen(charInputBuffer);
                    for(int i = length+1; i > textCursorPos; i--)
                        charInputBuffer[i] = charInputBuffer[i-1];
                    charInputBuffer[length] = 0;
                    charInputBuffer[textCursorPos] = ' ';

                }
                else if(in == KEY_HOME)
                    textCursorPos = 0;
                else if(in == KEY_END)
                {
                    int length = strlen(charInputBuffer);

                    if(length < SONGNAME_LENGTH)
                        textCursorPos = length;
                    else textCursorPos = SONGNAME_LENGTH - 1;
                }
                else
                {
                    if(in <= 'z' && in >= ' ')
                    {
                        int length = SONGNAME_LENGTH;
                        for(int i = length; i > textCursorPos; i--)
                            charInputBuffer[i] = charInputBuffer[i-1];
                        charInputBuffer[textCursorPos] = in;
                        charInputBuffer[length] = 0;
                        if(textCursorPos < SONGNAME_LENGTH-1)
                            textCursorPos++;
                    }
                }

                break;
            case 4:
            case 5:
                //Tracks ||Spinner [0,4]
                //Rows ||Spinner [0,5]
                if(!spinnervalid) return;
                switch(in)
                {
                    case KEY_UP:
                        numBuffer++;
                        break;
                    case KEY_LEFT:
                        numBuffer -= 0x10;
                        break;
                    case KEY_DOWN:
                        numBuffer--;
                        break;
                    case KEY_RIGHT:
                        numBuffer += 0x10;
                        break;
                    case KEY_END:
                        numBuffer = 1;
                        break;
                    case KEY_HOME:
                        numBuffer = 255;
                        break;
                    case KEY_DC:
                        numBuffer /= 0x10;
                        break;
                    default:
                        if(ishex)
                        {
                            numBuffer *= 0x10;
                            numBuffer += hexnum;
                            numBuffer &= 0xFF;
                        }
                }
                if(numBuffer > 255)
                    numBuffer = 0xFF;
                 

                break;
            case 6:
                //Resolution ||UInt Spinner [0,6]
                if(!spinnervalid) return;

                switch(in)
                {
                    case KEY_UP:
                        numBuffer += editor::song->getInterRowResolution();
                        numBuffer = (numBuffer/editor::song->getInterRowResolution())* editor::song->getInterRowResolution();
                        break;
                    case KEY_LEFT:
                        numBuffer -= 0x10*editor::song->getInterRowResolution();
                        numBuffer = (numBuffer/editor::song->getInterRowResolution())* editor::song->getInterRowResolution();
                        break;
                    case KEY_DOWN:
                        numBuffer -= editor::song->getInterRowResolution();
                        numBuffer = (numBuffer/editor::song->getInterRowResolution())* editor::song->getInterRowResolution();
                        break;
                    case KEY_RIGHT:
                        numBuffer += 0x10*editor::song->getInterRowResolution();
                        numBuffer = (numBuffer/editor::song->getInterRowResolution())* editor::song->getInterRowResolution();
                        break;
                    case KEY_END:
                        numBuffer = 0x100*editor::song->getInterRowResolution();
                        break;
                    case KEY_HOME:
                        numBuffer = 0xFFFFF;
                        numBuffer = (numBuffer/editor::song->getInterRowResolution())* editor::song->getInterRowResolution();
                        break;
                    case KEY_DC:
                        numBuffer = editor::song->getInterRowResolution();
                        break;
                    default:
                        if(ishex)
                        {
                            numBuffer *= 0x10;
                            numBuffer += hexnum;
                        }
                }
                if(numBuffer > 0xFFFF)
                    numBuffer &= 0xFFFF;
                else if(numBuffer < editor::song->getInterRowResolution())
                    numBuffer = editor::song->getInterRowResolution();
                break;
            case 7:
                //DIV  ||Spinner [0,7]
                if(!spinnervalid) return;
                switch(in)
                {
                    case KEY_UP:
                        numBuffer++;
                        break;
                    case KEY_LEFT:
                        numBuffer -= 0x8;
                        break;
                    case KEY_DOWN:
                        numBuffer--;
                        break;
                    case KEY_RIGHT:
                        numBuffer += 0x8;
                        break;
                    case KEY_END:
                        numBuffer = 1;
                        break;
                    case KEY_HOME:
                        numBuffer = 128;
                        break;
                    case KEY_DC:
                        numBuffer /= 0x10;
                        break;
                    default:
                        if(ishex)
                        {
                            numBuffer *= 0x10;
                            numBuffer += hexnum;
                            numBuffer &= 0xFF;
                        }
                }
                if(numBuffer > 255)
                    numBuffer = 0xFF;
                break;
                 
        }
    }
    else if(selrowmeta == 1)
    {
        switch(selobjmeta)
        {
            case 4:
                //Instrument ||Spinner [1,4]
                if(!spinnervalid) return;

                switch(in)
                {
                    case KEY_UP:
                        numBuffer++;
                        break;
                    case KEY_LEFT:
                        numBuffer -= 8;
                        break;
                    case KEY_DOWN:
                        numBuffer--;
                        break;
                    case KEY_RIGHT:
                        numBuffer += 8;
                        break;
                    case KEY_END:
                        numBuffer = 0;
                        break;
                    case KEY_HOME:
                        numBuffer = song->numInstruments()-1;
                        break;
                    case KEY_DC:
                        numBuffer /= 0x10;
                        break;
                    default:
                        if(ishex)
                        {
                            numBuffer *= 0x10;

                            if(numBuffer > song->numInstruments())
                                numBuffer = 0;

                            numBuffer += hexnum;

                            if(numBuffer >= song->numInstruments())
                                numBuffer = song->numInstruments()-1;

                            numBuffer &= 0xFF;
                        }
                        
                }
                
                break;
            case 5:
                //Step ||Spinner [1,5]
                if(!spinnervalid) return;

                switch(in)
                {
                    case KEY_UP:
                        numBuffer++;
                        break;
                    case KEY_DOWN:
                        numBuffer--;
                        break;
                    case KEY_END:
                        numBuffer = 0;
                        break;
                    case KEY_HOME:
                        numBuffer = 0xF;
                        break;
                    default:
                        if(ishex)
                            numBuffer = hexnum;
                }
                if(numBuffer >= 0xF)
                    numBuffer = 0xF;
                else if(numBuffer < 0)
                    numBuffer = 0;
                break;
            case 6:
                //Octave ||Spinner [1,6]
                if(!spinnervalid) return;
               
                switch(in)
                {
                    case KEY_UP:
                        numBuffer++;
                        break;
                    case KEY_DOWN:
                        numBuffer--;
                        break;
                    case KEY_END:
                        numBuffer = 0;
                        break;
                    case KEY_HOME:
                        numBuffer = 6;
                        break;
                    default:
                        if(ishex)
                            numBuffer = hexnum;
                }
                if(numBuffer >= 7)
                    numBuffer = 6;
                else if(numBuffer < 0)
                    numBuffer = 0;
                break;

            case 7:
                //Beat ||Spinner [1,8]
                if(!spinnervalid) return;
               
                switch(in)
                {
                    case KEY_UP:
                        numBuffer++;
                        break;
                    case KEY_DOWN:
                        numBuffer--;
                        break;
                    case KEY_END:
                        numBuffer = 0;
                        break;
                    case KEY_HOME:
                        numBuffer = 6;
                        break;
                    default:
                        if(ishex)
                            numBuffer = hexnum;
                }
                if(numBuffer >= 0xF)
                    numBuffer = 0xF;
                else if(numBuffer < 0)
                    numBuffer = 0;
                break;
        }

    }
    else if(selrowmeta == 2)
    {
        switch(selobjmeta)
        {
            case 0:
            case 1:
                //Order Selected ||Spinner [2,0]
                //Pattern Selected ||Spinner [2,1]
                if(!spinnervalid) return;
            switch(in)
                {
                    case KEY_UP:
                        numBuffer++;
                        break;
                    case KEY_LEFT:
                        numBuffer -= 0x10;
                        break;
                    case KEY_DOWN:
                        numBuffer--;
                        break;
                    case KEY_RIGHT:
                        numBuffer += 0x10;
                        break;
                    case KEY_END:
                        numBuffer = 0;
                        break;
                    case KEY_HOME:
                        numBuffer = 255;
                        break;
                    case KEY_DC:
                        numBuffer /= 0x10;
                        break;
                    default:
                        if(ishex)
                        {
                            numBuffer *= 0x10;
                            numBuffer += hexnum;
                        }
                }
            if(selobjmeta == 0)
            {
                if(numBuffer >= song->numOrders())
                    numBuffer = song->numOrders()-1;
                else if(numBuffer < 0)
                    numBuffer = 0;          
            }
            else if(selobjmeta == 1)
            {
                if(numBuffer >= song->numPatterns())
                    numBuffer = song->numPatterns()-1;
                else if(numBuffer < 0)
                    numBuffer = 0;           
            }

            break;
            case 13:
                //Key ||Interactable?
               
                switch(in)
                {
                    case KEY_UP:
                        numBuffer++;
                        break;
                    case KEY_DOWN:
                        numBuffer--;
                        break;
                    case KEY_END:
                        numBuffer = 0;
                        break;
                    case KEY_HOME:
                        numBuffer = CHROMATIC_NOTES-1;
                        break;
                    case KEY_DC:
                    case ALT_BACKSPACE:
                    case ' ':
                        for(int i = 0; i < CHROMATIC_NOTES-1; i++)
                            scaleconst[i] = 1;
                        scalespinner = inferScaleType(scaleconst);

                    case 'q':
                        numBuffer = 0;//C
                        break;
                    case '2':
                        numBuffer = 1;//C#
                        break;
                    case 'w':
                        numBuffer = 2;//D
                        break;
                    case '3':
                        numBuffer = 3;//D#
                        break;
                    case 'e':
                        numBuffer = 4;//E
                        break;
                    case 'r':
                        numBuffer = 5;//F
                        break;
                    case '5':
                        numBuffer = 6;//F#
                        break;
                    case 't':
                        numBuffer = 7;//G
                        break;
                    case '6':
                        numBuffer = 8;//G#
                        break;
                    case 'y':
                        numBuffer = 9;//A
                        break;
                    case '7':
                        numBuffer = 10;//A#
                        break;
                    case 'u':
                        numBuffer = 11;//B
                        break;
                }
                if(numBuffer >= CHROMATIC_NOTES-1)
                    numBuffer = CHROMATIC_NOTES-1;
                else if(numBuffer < 0)
                    numBuffer = 0;
                break;
            case 16:
                //Scale ||Interactable
                switch(in)
                {
                    case KEY_LEFT:
                        if(textCursorPos > 0)
                            textCursorPos--;
                        break;
                    case KEY_RIGHT:
                        textCursorPos++;
                        break;
                    case KEY_DC:
                    case ALT_BACKSPACE:
                        for(int i = 0; i < CHROMATIC_NOTES-1; i++)
                            scaleconst[i] = 1;
                        key = 0;
                        scalespinner = inferScaleType(scaleconst);
                        break;

                    default:
                        if(in == KEY_DC)
                        {
                            hexnum = 1;
                            ishex = true;
                        }

                        if(ishex && hexnum > 0 && hexnum < 0xA)
                        {
                            scaleconst[textCursorPos] = hexnum;
                            textCursorPos++;
                            int acc, i;
                            for(acc = 0, i = 0; acc < CHROMATIC_NOTES && i < CHROMATIC_NOTES-1 && scaleconst[i] != 0; i++)
                                acc += scaleconst[i];
                            if(acc > CHROMATIC_NOTES)
                                scaleconst[i-1] -= acc - CHROMATIC_NOTES;
                            if(acc < CHROMATIC_NOTES)
                                for(; acc < CHROMATIC_NOTES; i++, acc++)
                                    scaleconst[i]=1;
                            for(;i < CHROMATIC_NOTES; i++)
                                scaleconst[i] = 0;
                        }
                }
                int acc, i;
                for(acc = 0, i = 0; acc < CHROMATIC_NOTES && i < CHROMATIC_NOTES-1; i++)
                    acc += scaleconst[i];
                if(textCursorPos >= i)
                    textCursorPos = i-1;
                scalespinner = inferScaleType(scaleconst);


        }

    }


}

void patternedtr::doneMetaEditing()
{
    //Commit changes
    using namespace editor;
    if(selrowmeta == 0)
    {
        switch(selobjmeta)
        {
            case 3:
                //copy input buffer into song name
                copy(charInputBuffer, song->getName(), SONGNAME_LENGTH);
                break;
            case 4:
                //set tracknum to buffer if valid
                //
                if(numBuffer < 1)
                    numBuffer = 1;
                //fix muted_tracks
                trackNumChanged(song->numTracks(), numBuffer);
                song->setTrackNum(numBuffer);
                break;
            case 5:
                //Set pattern's row number to buffer
                if(numBuffer < 1)
                    numBuffer = 1;
                selptrn->setRowNum(numBuffer);
                patternedtr::viewportrow=0;
                patternedtr::selrow=0;
                break;
            case 6:
                //set BPR to buffer
                if(numBuffer < 16)
                    numBuffer = 16;//who in the right mind would decrease the resolution any further?
                song->setBytesPerRow(numBuffer);
                break;
            case 7:
                //set segment number to buffer
                if(numBuffer < 1)
                    numBuffer = 1;
                song->setInterRowResolution(numBuffer);
                break;
        }
    }
    else if(selrowmeta == 1)
    {
        switch(selobjmeta)
        {
            case 4:
                //set selected instrument to new instrument
                if(numBuffer >= song->numInstruments())
                    numBuffer = song->numInstruments()-1;
                else if(numBuffer < 0)
                    numBuffer = 0;
                selinstrument = numBuffer;
                selinst = song->getInstrument(selinstrument);
                break;
            case 5:
                edit_step = numBuffer;
                break;
            case 6:
                octave = numBuffer;
                break;
            case 7:
                row_underline = numBuffer;
                break;

        }
    }
    else if(selrowmeta == 2)
    {
        switch(selobjmeta)
        {
            case 0:
                selorder = numBuffer;
                selptrn = song->getPatternByOrder(selorder);
                break;
            case 1:
                song->setPatternIndexByOrder(selorder, numBuffer);
                selptrn = song->getPattern(numBuffer);
                break;
            case 13:
                key = numBuffer;
                populateNoteMap();
                break;
            case 16:
                populateNoteMap();
                break;
        }
    }
    metaobjedit = false;
}


void patternedtr::processMetaInput(wint_t in)
{

    if(!metaobjedit)
    {
        bool ishex, hexnum;
        switch(in)
        {
            case KEY_UP:
                chgSelMetaRow(-1);
                break;
            case KEY_DOWN:
                chgSelMetaRow(1);
                break;
            case KEY_LEFT:
                chgSelMetaObj(-1);
                break;
            case KEY_RIGHT:
                chgSelMetaObj(1);
                break;
            case KEY_HOME:
                chgSelMetaObj(-int(selobjmeta));
                break;
            case KEY_END:
                chgSelMetaObj(20);
                break;
            case '\n':
                //Either fire an button event or start editing
                startMetaEditing();
                break;
            case KEY_NPAGE:
                editor::inputwin = editor::ptrnwin;
                editor::wingroup = editor::ptrnwin;
                break;
            default:
                ishex =  editor::validateHexChar(in);
                if(ishex)
                {
                    //Some objects accept hex input without being _edited_
                    if( selrowmeta == 0 && selobjmeta == 6 ||
                            selrowmeta == 1 && (selobjmeta == 4 || selobjmeta == 5 || selobjmeta == 6) ||
                            selrowmeta == 2 && (selobjmeta == 0 || selobjmeta == 1)
                      )
                        metaEdit(in);
                }

        }
    }
    else
    {
        if(in == '\n')
            doneMetaEditing();
        else
            metaEdit(in);

    }



}



void editor::trackNumChanged(unsigned int from, unsigned int to)
{
    bool *newmute = new bool[to];
    int min = from;
    if(min > to)
        min = to;

    for(int i = 0; i < min; i++)
        newmute[i] = editor::muted_tracks[i];

    delete[] muted_tracks;
    muted_tracks = newmute;
    patternedtr::seltrack = 0;
    patternedtr::viewporttrack = 0;
}


void patternedtr::newSong()
{
    delete editor::song;
    editor::selinst = NULL;

    patternedtr::viewporttrack = 0;
    patternedtr::viewportrow = 0;
    patternedtr::seltrack = 0;
    patternedtr::selrow = 0;

    patternedtr::selorder = 0;

    instedtr::selwavrow = 0;

    editor::song = new Song();

    bool *newmute = new bool[editor::song->numTracks()];
    for(int i = 0; i < editor::song->numTracks(); i++)
        newmute[i] = 0;

    delete[] editor::muted_tracks;
    editor::muted_tracks = newmute;

    patternedtr::selinstrument = 0;
    editor::selinst = editor::song->getInstrument(0);
    patternedtr::selptrn = editor::song->getPatternByOrder(0);


    using editor::lastSongPath;
    for(int i = 8; i < LASTSONG_SIZE; i++)
        lastSongPath[i] = 0;
    lastSongPath[0] = 's';
    lastSongPath[1] = 'o';
    lastSongPath[2] = 'n';
    lastSongPath[3] = 'g';
    lastSongPath[4] = '.';
    lastSongPath[5] = 'p';
    lastSongPath[6] = 'l';
    lastSongPath[7] = 'b';


}

bool patternedtr::saveSong(const char *file)
{
    //Store the name
    int namelen = strlen(file);
    if(namelen < LASTSONG_SIZE)
        editor::copy(file, editor::lastSongPath, strlen(file));
    else
        editor::copy(file, editor::lastSongPath, LASTSONG_SIZE);

    //Open the filestream
    std::ofstream fileout(file);
    if(!fileout)
        return false;

    //output the song
    editor::song->output(fileout);
    fileout.close();
    return true;
}

bool patternedtr::openSong(const char *file)
{
    int namelen = strlen(file);
    if(namelen < LASTSONG_SIZE)
        editor::copy(file, editor::lastSongPath, strlen(file));
    else
        editor::copy(file, editor::lastSongPath, LASTSONG_SIZE);

    std::ifstream filein(file);
    if(!filein)
        return false;
    if(editor::song == NULL)
        editor::song = new Song(filein);
    else
        editor::song->input(filein);

    filein.close();

    bool *newmute = new bool[editor::song->numTracks()];
    for(int i = 0; i < editor::song->numTracks(); i++)
        newmute[i] = 0;

    delete[] editor::muted_tracks;
    editor::muted_tracks = newmute;

    selorder = 0;
    viewporttrack=0;
    viewportrow=0;
    selrow = 0;
    seltrack=0;
    selinstrument=0;
    editor::selinst = editor::song->getInstrument(0);
    instedtr::selwavrow=0;
    instedtr::viewportwave = 0;
    instedtr::selvolrow=0;
    instedtr::viewportvol = 0;

    selptrn = editor::song->getPatternByOrder(selorder);
    return true;
}

void patternedtr::newInstrument()
{
    bool success = editor::song->newInstrument();
    if(success)
    {
        selinstrument = editor::song->numInstruments()-1;
        editor::selinst = editor::song->getInstrument(selinstrument);
    }
    else editor::inform("New Instrument failed, too many Instruments");
}


void patternedtr::cloneInstrument(unsigned char inst)
{
    bool success = editor::song->cloneInstrument(inst);
    if(success)
    {
        selinstrument = editor::song->numInstruments()-1;
        editor::selinst = editor::song->getInstrument(selinstrument);
    }
    else editor::inform("Clone Instrument failed, too many Instruments");

}
void patternedtr::removeInstrument(unsigned char inst)
{
    using namespace editor;
    bool success = editor::song->removeInstrument(inst);
    if(success)
        selinstrument--;
    else
        editor::inform("Can't Remove that Instrument                 ");
    if(selinstrument > song->numInstruments())
        selinstrument = 0;
    selinst = song->getInstrument(selinstrument);
    //TODO make sure instrument set to valid inst
}
void patternedtr::chgSelOrder(int i) 
{
    using namespace editor;
    selorder += i;
    if(selorder >= song->numOrders())
    {
        if(i > 0)
            selorder = song->numOrders()-1;
        else
            selorder = 0;
    }
    selptrn = editor::song->getPattern(editor::song->getPatternIndexByOrder(selorder));
}

void patternedtr::newOrder() 
{
    using namespace editor;
    if(selorder < 255)
    {//TODO make error messages if cant make pattern etc.
        bool success = editor::song->newPattern(song->numTracks(), selptrn->numRows());
        if (success)
        {
            editor::song->insertOrder(selorder+1, editor::song->numPatterns()-1);
            selorder++;
            selptrn = editor::song->getPattern(editor::song->getPatternIndexByOrder(selorder));

        }
        else
            editor::inform("Too many Patterns to make a new Pattern for the New Order");
    }
    else editor::inform("Too many orders, can't make New Order             ");
}

void patternedtr::cloneOrder(unsigned char order) 
{
    using namespace editor;
    bool success = editor::song->insertOrder(selorder+1, editor::song->getPatternIndexByOrder(order));
    if(success)
    {
        selorder = editor::song->numOrders()-1;
        selptrn = editor::song->getPattern(editor::song->getPatternIndexByOrder(selorder));
    }
    else editor::inform("Failed to Insert a new order                      ");
}

void patternedtr::removeOrder(unsigned char order) 
{
    using namespace editor;
    bool success = editor::song->removeOrder(order);
    if(success)
    {
        if(selorder >= editor::song->numOrders())
            selorder--;
        selptrn = editor::song->getPattern(editor::song->getPatternIndexByOrder(selorder));
    }
    else editor::inform("Can't Remove the last order                         ");
}

void patternedtr::chgOrderPattern(int i) 
{
    using namespace editor;
    unsigned char sel = editor::song->getPatternIndexByOrder(selorder);
    sel += i;
    if(sel >= editor::song->numPatterns())
        if(i>0)
            sel = editor::song->numPatterns()-1;
        else
            sel = 0;
    editor::song->setPatternIndexByOrder(selorder, sel);
    selptrn = editor::song->getPattern(sel);
}



void patternedtr::newPattern()
{
    using namespace editor;
    bool success = editor::song->newPattern(song->numTracks(), selptrn->numRows());
    if(success)
    {
        unsigned char newpat = editor::song->numPatterns() - 1;
        editor::song->setPatternIndexByOrder(selorder, newpat);
        selptrn = editor::song->getPattern(newpat);

    }
    else
        editor::inform("New Pattern failed, at maximum");
}
void patternedtr::clonePattern(unsigned char ptrnsrc)
{
    using namespace editor;
    bool success = editor::song->clonePattern(ptrnsrc);
    if(success)
    {
        unsigned char newpat = editor::song->numPatterns() - 1;
        editor::song->setPatternIndexByOrder(selorder, newpat);
        selptrn = editor::song->getPattern(newpat);
    }
    else
        editor::inform("Clone Pattern failed, too many Patterns");
}

void patternedtr::clearPattern(unsigned char ptrn)
{
    editor::song->clearPattern(ptrn);
}

void patternedtr::removePattern(unsigned char ptrn)
{
    using namespace editor;
    bool success = editor::song->removePattern(ptrn);

    if(success)
    {
        //the order changes its patterns to the nearest pattern.
        //update to it:
        selptrn = editor::song->getPatternByOrder(selorder);
    }
    else
        editor::inform("Remove Pattern failed");

}

void patternedtr::clearTrack(unsigned char track)
{
    for(int i = 0; i <selptrn->numRows(); i++)
        selptrn->setAt(track, i, R_EMPTY);
}




void patternedtr::exit()
{
    editor::stopPlayback();
    editor::running = false;
}





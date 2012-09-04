#include "codeeditor.h"
#include "syntaxhighligher.h"
#include "linenumberarea.h"
#include "dialogs/gotolinedialog.h"
#include "util/widgethelper.h"
#include <QPainter>

CodeEditor::CodeEditor(QWidget *parent) :
    QPlainTextEdit(parent)
{
    QFont monospaceFont("Monospace");
    setFont(monospaceFont);

    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    setTabStopWidth(30);
    setWordWrapMode(QTextOption::NoWrap);
    new SyntaxHighligher(this->document());

    QPalette p=palette();
    p.setColor(QPalette::Disabled, QPalette::Base, p.color(QPalette::Window));
    setPalette(p);

    strTab=QString("  ");

    setFont(QFont("Monospace"));
}

//use only when sure that there's not lots of text in editor
QStringList CodeEditor::getSemicolonSeparated() const
{
    return toPlainText().split(';', QString::SkipEmptyParts);
}

int CodeEditor::lineNumberAreaWidth()
 {
     int digits = 1;
     int max = qMax(1, blockCount());
     while (max >= 10) {
         max /= 10;
         ++digits;
     }

     int space = 5 + 5 + fontMetrics().width(QLatin1Char('9')) * digits;

     return space;
 }



 void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
 {
     setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
 }



 void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
 {
     if (dy)
         lineNumberArea->scroll(0, dy);
     else
         lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

     if (rect.contains(viewport()->rect()))
         updateLineNumberAreaWidth(0);
 }

 void CodeEditor::resizeEvent(QResizeEvent *e)
 {
     QPlainTextEdit::resizeEvent(e);

     QRect cr = contentsRect();
     lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
 }



 void CodeEditor::highlightCurrentLine()
 {
     QList<QTextEdit::ExtraSelection> extraSelections;

     if (!isReadOnly()) {
         QTextEdit::ExtraSelection selection;

         QColor lineColor = QColor(Qt::yellow).lighter(180);

         selection.format.setBackground(lineColor);
         selection.format.setProperty(QTextFormat::FullWidthSelection, true);
         selection.cursor = textCursor();
         selection.cursor.clearSelection();
         extraSelections.append(selection);
     }

     for(int i=0; i<foundTextPositions.size(); ++i){
         QTextEdit::ExtraSelection searchTextSelection;
         searchTextSelection.format.setBackground(QColor(Qt::yellow));
         searchTextSelection.format.setForeground(palette().color(QPalette::HighlightedText));
         searchTextSelection.cursor=foundTextPositions.at(i);

         extraSelections.append(searchTextSelection);
     }

     setExtraSelections(extraSelections);
 }



 void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
 {
     QPainter painter(lineNumberArea);
     QRect rect=event->rect();
     painter.fillRect(rect, palette().color(QPalette::Base));
     painter.setPen(Qt::DotLine);
     painter.drawLine(rect.right(), rect.top(), rect.right(), rect.bottom());


     QTextBlock block = firstVisibleBlock();
     int blockNumber = block.blockNumber();
     int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
     int bottom = top + (int) blockBoundingRect(block).height();

     while (block.isValid() && top <= event->rect().bottom()) {
         if (block.isVisible() && bottom >= event->rect().top()) {
             QString number = QString::number(blockNumber + 1);
             painter.drawText(0, top, lineNumberArea->width()-5, fontMetrics().height(),
                              Qt::AlignRight, number);
         }

         block = block.next();
         top = bottom;
         bottom = top + (int) blockBoundingRect(block).height();
         ++blockNumber;
     }
 }

 void CodeEditor::keyReleaseEvent ( QKeyEvent * event )
 {
     QPlainTextEdit::keyReleaseEvent(event);

     if(event->key()==Qt::Key_Escape){
        emit escapeKeyPressed();
     }
 }

 void CodeEditor::keyPressEvent ( QKeyEvent * event )
 {
     bool handled=false;

     if(!isReadOnly()){
         int key=event->key();
         if(key==Qt::Key_Return || key==Qt::Key_Enter){
            handled=true;
            textCursor().beginEditBlock();
            QPlainTextEdit::keyPressEvent(event);
            autoIndentNewBlock();
            textCursor().endEditBlock();
         }else if(key==Qt::Key_Tab){
             handled=true;

             if(textCursor().hasSelection()){
                 if(event->modifiers() & Qt::ShiftModifier){
                     unindentSelection();
                 }else{
                     indentSelection();
                 }
             }else{
                 textCursor().insertText(strTab);
             }
         }else if(key==Qt::Key_Backtab && textCursor().hasSelection()){
             handled=true;
             unindentSelection();
         }else if(key==Qt::Key_Home && (Qt::ControlModifier & event->modifiers())!=Qt::ControlModifier){
             handled=true;
             handleHomeKey(Qt::ShiftModifier & event->modifiers());
         }
     }

     if(!handled){
        QPlainTextEdit::keyPressEvent(event);
     }
 }

 void CodeEditor::autoIndentNewBlock()
 {
     QTextCursor cur=textCursor();
     if(!cur.movePosition(QTextCursor::PreviousBlock)){
         return;
     }
     QString prevLine=cur.block().text();
     QString prefix;
     QChar c;
     for(int i=0; i<prevLine.size(); ++i){
         c=prevLine.at(i);
         if(!c.isSpace()){
             break;
         }
         prefix.append(c);
     }
     cur.movePosition(QTextCursor::NextBlock);
     if(!prefix.isEmpty()){
        cur.insertText(prefix);
        setTextCursor(cur);
     }

 }

 void CodeEditor::indentSelection()
 {
     QTextCursor cur=textCursor();
     if(!cur.hasSelection()){
         return;
     }
     cur.beginEditBlock();
     CursorPositionInfo inf=getStartStopPositions(cur);

     cur.setPosition(inf.startPos);
     //cur.movePosition(QTextCursor::StartOfBlock);
     int blocksToMove=(inf.endBlock-inf.startBlock)+1;
     for(int i=0; i<blocksToMove; ++i){
        cur.insertText(strTab);
        cur.movePosition(QTextCursor::NextBlock);
     }
     cur.endEditBlock();

     inf.startPos+=strTab.size();
     inf.endPos+=blocksToMove*strTab.size();
     inf.selectText(cur);

     setTextCursor(cur);
 }

 void CodeEditor::unindentSelection()
 {
     QTextCursor cur=textCursor();
     if(!cur.hasSelection()){
         return;
     }
     cur.beginEditBlock();
     CursorPositionInfo inf=getStartStopPositions(cur);

     cur.setPosition(inf.startPos);
     //cur.movePosition(QTextCursor::StartOfBlock);
     int blocksToMove=(inf.endBlock-inf.startBlock)+1;
     for(int i=0; i<blocksToMove; ++i){
        moveToFirstNonSpaceCharacter(cur);
        for(int k=0; k<strTab.size(); ++k){
            if(cur.positionInBlock()>0){
                cur.deletePreviousChar();
                if(i==0){
                    --inf.startPos;
                }

                --inf.endPos;
            }else{
                break;
            }
        }

        cur.movePosition(QTextCursor::NextBlock);
     }
     cur.endEditBlock();
     inf.selectText(cur);
     setTextCursor(cur);
 }

 void CodeEditor::handleHomeKey(bool keepSelection)
 {
     QTextCursor cur=textCursor();
     bool onlySpaces;
     int ix=getFirstNonSpaceCharacterIndex(cur, &onlySpaces);
     if(!cur.atBlockStart() && cur.positionInBlock()<=ix){
         cur.movePosition(QTextCursor::StartOfBlock, keepSelection ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor);
     }else{
         moveToFirstNonSpaceCharacter(cur, keepSelection ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor);
     }
     setTextCursor(cur);
 }

 int CodeEditor::getFirstNonSpaceCharacterIndex(QTextCursor &cur, bool *onlySpaces)
 {
     QString line=cur.block().text();
     QChar c;
     int firstNonSpaceIx=-1;
     for(int i=0; i<line.size(); ++i){
         c=line.at(i);
         if(!c.isSpace()){
             firstNonSpaceIx=i;
             break;
         }
     }

     if(onlySpaces!=0){
        *onlySpaces=(firstNonSpaceIx==-1 && line.size()>0);
         if(*onlySpaces){
             firstNonSpaceIx=line.size();
         }
     }
     return firstNonSpaceIx;
 }

 bool CodeEditor::moveToFirstNonSpaceCharacter(QTextCursor &cur, QTextCursor::MoveMode moveMode)
 {
    bool onlySpaces;
    int ix = getFirstNonSpaceCharacterIndex(cur, &onlySpaces);
    cur.setPosition(cur.block().position()+ix, moveMode);
    return !onlySpaces;
 }

 void CodeEditor::removeFromBeginning(QTextCursor &cur, int characterCount){
     moveToFirstNonSpaceCharacter(cur);
     for(int k=0; k<characterCount; ++k){
         cur.deleteChar();
     }
 }

 void CodeEditor::changeCase(bool toUpper)
 {
     QTextCursor cur=textCursor();
     int startPos=cur.anchor();
     if(cur.hasSelection()){
        int endPos=cur.position();
        cur.insertText(toUpper ? cur.selectedText().toUpper() : cur.selectedText().toLower());
        cur.setPosition(startPos);
        cur.setPosition(endPos, QTextCursor::KeepAnchor);
     }else{
         cur.select(QTextCursor::WordUnderCursor);
         if(cur.hasSelection()){
             cur.insertText(toUpper ? cur.selectedText().toUpper() : cur.selectedText().toLower());
         }
         cur.setPosition(startPos);
     }
     setTextCursor(cur);
 }

 CursorPositionInfo CodeEditor::getStartStopPositions(const QTextCursor cur)
 {
     CursorPositionInfo info;

     int startPos=cur.anchor();
     int endPos=cur.position();
     if(startPos>endPos){
         qSwap(startPos, endPos);
         info.anchorAtEnd=true;
     }else{
         info.anchorAtEnd=false;
     }
     int startBlock=cur.document()->findBlock(startPos).blockNumber();
     int endBlock=cur.document()->findBlock(endPos).blockNumber();

     info.startPos=startPos;
     info.endPos=endPos;
     info.startBlock=startBlock;
     info.endBlock=endBlock;

     return info;
 }

 void CodeEditor::commentBlocks()
 {
     QString comment="--";
     QTextCursor cur=textCursor();
     CursorPositionInfo inf=getStartStopPositions(cur);

     cur.setPosition(inf.startPos);
     int blocksToMove=(inf.endBlock-inf.startBlock)+1;
     int addComments=-1;

     cur.beginEditBlock();

     /*
    int minPosToStart=-1;
    for(int i=0; i<blocksToMove; ++i){
        bool onlySpaces;
        int blockMin=getFirstNonSpaceCharacterIndex(cur, &onlySpaces);
        if(minPosToStart==-1){
            minPosToStart=blockMin;
        }
        if(!onlySpaces && blockMin<minPosToStart){
            minPosToStart=blockMin;
        }
        cur.movePosition(QTextCursor::NextBlock);
    }*/

     cur.setPosition(inf.startPos);
     for(int i=0; i<blocksToMove; ++i){
         QString blockText=cur.block().text().trimmed();
         if(!blockText.isEmpty()){
             if(addComments==-1){
                 addComments=blockText.startsWith(comment) ? 0 : 1;
             }

             if(addComments==1){
                 cur.setPosition(cur.block().position());
                 cur.insertText(comment);
                 inf.endPos+=comment.size();
             }else if(addComments==0 && blockText.startsWith(comment)){
                 removeFromBeginning(cur, comment.size());
                 inf.endPos-=comment.size();
             }
         }
         cur.movePosition(QTextCursor::NextBlock);
     }
     if(addComments==1){
         inf.startPos+=comment.size();
     }else if(addComments==0){
         inf.startPos-=comment.size();
     }
     cur.endEditBlock();
     inf.selectText(cur);
     setTextCursor(cur);
 }

 void CodeEditor::goToLine()
 {
     QTextCursor cur=textCursor();
     GoToLineDialog dialog(cur.blockNumber()+1, document()->blockCount(), this);
     dialog.setWindowModality(Qt::WindowModal);
     if(dialog.exec()){
         int line = dialog.getLine()-1;
         cur.setPosition(document()->findBlockByNumber(line).position());
         setTextCursor(cur);
         ensureCursorVisible();
     }
 }

 void CodeEditor::increaseFontSize()
 {
    WidgetHelper::changeFontSize(this, 0.5);
 }

 void CodeEditor::decreaseFontSize()
 {
     WidgetHelper::changeFontSize(this, -0.5);
 }

 void CodeEditor::toUpperCase()
 {
     changeCase(true);
 }

 void CodeEditor::toLowerCase()
 {
     changeCase(false);
 }

 void CodeEditor::moveUp()
 {
    moveSelection(true);
 }

 void CodeEditor::moveDown()
 {
     moveSelection(false);
 }

 void CodeEditor::moveSelection(bool up)
 {
     QTextCursor cur=textCursor();
     CursorPositionInfo inf=getStartStopPositions(cur);
     cur.movePosition(QTextCursor::StartOfBlock);
     cur.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
     QString text=cur.selectedText();

     cur.beginEditBlock();
     cur.removeSelectedText();
     if(up && inf.startBlock>0){
        int prevBlockPos=document()->findBlockByNumber(inf.startBlock-1).position();
        cur.setPosition(prevBlockPos);
        cur.insertText(QString("%1\n").arg(text));
        cur.setPosition(prevBlockPos);
        cur.setPosition(prevBlockPos+text.size(), QTextCursor::KeepAnchor);
     }else if(!up && inf.endBlock<document()->blockCount()-2){
        int nextBlockPos=document()->findBlockByNumber(inf.endBlock+2).position();
        cur.setPosition(nextBlockPos);
        cur.insertText(QString("%1\n").arg(text));
        cur.setPosition(nextBlockPos);
        cur.setPosition(nextBlockPos+text.size(), QTextCursor::KeepAnchor);
     }
     cur.endEditBlock();
 }

 void CodeEditor::addText(const QString &text)
 {
     QTextCursor cur=this->textCursor();
     if(!cur.atEnd()){
         cur.movePosition(QTextCursor::End);
     }
     cur.insertText(text);
 }

 QString CodeEditor::getCurrentText() const
 {
     QTextCursor cursor=textCursor();
     if(cursor.hasSelection()){
         return cursor.selectedText().replace(QChar(0x2029), QChar('\n'));
     }else{
         return toPlainText();
     }
 }

 void CodeEditor::setFoundTextPositions(const QList< QTextCursor > &foundTextPositions)
 {
     if(foundTextPositions.size()>0){
        this->foundTextPositions=foundTextPositions;
        highlightCurrentLine();
     }
 }

 void CodeEditor::clearFoundTextPositions()
 {
     if(this->foundTextPositions.size()>0){
        this->foundTextPositions.clear();
        highlightCurrentLine();
     }
 }

 void CodeEditor::showLinePosition(int line, int linePos)
 {
     QTextBlock block=this->document()->findBlockByNumber(line);
     QTextCursor cursor=this->textCursor();
     cursor.setPosition(block.position()+linePos);
     setTextCursor(cursor);
     ensureCursorVisible();
     setFocus();
 }

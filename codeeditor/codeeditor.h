#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QTime>
#include "cursorpositioninfo.h"
#include "typedefs.h"

class LineNavigationBar;
class CodeCollapseArea;
class QCompleter;
class CodeCollapsePosition;
class ToolTipWidget;
class IQueryScheduler;
class PlSqlTreeBuilder;
class QAbstractItemModel;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    enum CaseFoldingType
    {
        NoCaseFolding,
        UpperCaseFolding,
        LowerCaseFolding
    };

    CodeEditor(bool plsqlMode = false, QWidget *parent = 0);
    virtual ~CodeEditor();

    void setQueryScheduler(IQueryScheduler *queryScheduler);
    IQueryScheduler *getQueryScheduler() const;

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth() const;
    //void lineNumberAreaWheelEvent(QWheelEvent * event);

    void codeCollapseAreaPaintEvent(QPaintEvent *event);
    int codeCollapseAreaWidth() const;
    void refreshCodeCollapseArea();

    void lineNavBarPaintEvent(QPaintEvent *event);
    void lineNavBarMouseReleaseEvent(QMouseEvent * event);
    void lineNavBarWheelEvent(QWheelEvent * event);

    //use only when sure that there's not lots of text in editor
    QStringList getSemicolonSeparated() const;

    void addText(const QString &text);
    QString getCurrentText(QTextCursor &txtCursor, bool fullTextOnNoSelection=false) const;
    QString getCurrentTextSurroundedByEmptyLines(QTextCursor &cursor) const;

    void setFoundTextPositions(const QList< QTextCursor > &foundTextPositions);
    void clearFoundTextPositions();

    void showLinePosition(int line, int linePos);

    void pulsate(const QTextCursor &cursor, bool makeVisible = false, int duration = 300);
    void pulsate(int startPos, int endPos, bool makeVisible, int duration = 300);
    void ensureVisible(const QTextCursor &cursor);

    void setErrorPosition(const QTextCursor &cursor);
    void setErrorPositions(const QList< QTextCursor > &errorPositions);
    void addErrorPosition(const QTextCursor &cursor);

    void setCurrentIdentifierPositions(const QList< QTextCursor > &identifierPositions);
    void clearCurrentIdentifierPositions();

    void setMarkedLine(int line);

    void setCompleter(QCompleter *completer);
    //QCompleter *completer() const;

    void setCodeCollapsePositions(QList<CodeCollapsePosition*> *collapsePositions);

    virtual bool eventFilter ( QObject * watched, QEvent * event );

    bool blockChanges() const;
    void setBlockChanges(bool block);

    void setLastParseId(int parseId, PlSqlTreeBuilder *treeBuilder);
    int getLastParseId() const;

    PlSqlTreeBuilder *getTreeBuilder() const;

    void updateAllParts();

    void setDocument(QTextDocument * document);

    bool isPlsqlMode() const;

    CursorPositionInfo getStartStopPositions(const QTextCursor &cur);

    void setPairEditor(CodeEditor *pairEditor);
    CodeEditor *getPairEditor() const;

    void describeObject();
    void describeObject(const QTextCursor &cur);
    bool describeLocalObject();
    bool describeLocalObject(const QTextCursor &cur);

    static QList<CodeEditor*> openEditors;

    static QStringHash textShortcuts;
    static void loadTextShortcuts();

    static CaseFoldingType keywordCaseFolding;
    static CaseFoldingType identifierCaseFolding;

public slots:
    void commentBlocks();
    void goToLine();
    void increaseFontSize();
    void decreaseFontSize();
    void resetFontSize();
    void toUpperCase();
    void toLowerCase();
    void makeDuplicate();
    void removeEmptyLines();
    void moveUp();
    void moveDown();
    void moveSelectedText(bool up);
    void selectCurrentBlock();
    void applyCaseFolding();
    void formatCode();

    void customCut();
    void customCopy();

    void handleTextChanged();

    void undo();
    void redo();

    void timerTick();
signals:
    void escapeKeyPressed();
    void gotFocus();
    void lostFocus();
    void needsCompletionList(bool terminatedBySeparator);
    void updated(CodeEditor *editor);
    void applyCaseFoldingRequested();
    void switchToPair();
    void needsToDescribeObject(const QString &objectName);

protected:
    void resizeEvent(QResizeEvent *event);
    void keyReleaseEvent ( QKeyEvent * event );
    void keyPressEvent ( QKeyEvent * event );
    void focusInEvent ( QFocusEvent * event);
    void focusOutEvent ( QFocusEvent * event);
    void contextMenuEvent ( QContextMenuEvent * event );
    bool event ( QEvent * e );
    //virtual void paintEvent ( QPaintEvent * event );

    void timerEvent(QTimerEvent *event);

public slots:
    void completionModelReady(QAbstractItemModel *model, int cursorPosition);

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void setUndoAvailable(bool available);
    void setRedoAvailable(bool available);
    void removePulsatePositions();
    void updateNavBar();

    void insertCompletion(const QString &completion);

private:
    QWidget *lineNumberArea;
    CodeCollapseArea *codeCollapseArea;
    LineNavigationBar *lineNavBar;

    IQueryScheduler *queryScheduler;

    QTime codeCollapseAreaMouseEnterTime;

    QList< QTextCursor > foundTextPositions;
    QList< QTextCursor > pulsatePositions;
    QList< QTextCursor > errorPositions;
    QList< QTextCursor > currentIdentifierPositions;
    QTextCursor collapsibleRegionPositions;
    int collapsibleRegionStartBlockNumber;
    QTextCursor clickablePositions;

    static QString strTab;

    int markedLineIx;

    int pulsateTimerId;

    void autoIndentNewBlock(int cursorPosInBlock);
    int findLastUnclosedBracePosition(const QTextBlock &block) const;
    void indentSelection();
    void unindentSelection();
    void insertTab();
    bool insertTextShortcut(QTextCursor &cur);
    void handleHomeKey(bool keepSelection);
    int getFirstNonSpaceCharacterIndex(QTextCursor &cur, bool *onlySpaces=0);
    bool moveToFirstNonSpaceCharacter(QTextCursor &cur, QTextCursor::MoveMode moveMode=QTextCursor::MoveAnchor);
    void removeFromBeginning(QTextCursor &cur, int characterCount);
    void changeCase(bool toUpper);
    void moveSelectionUp();
    void moveSelectionDown();
    void ensureHasSelection();

    void drawLineNavBarHighlights(QPainter &painter);
    void drawLineNavBarHighlight(QPainter &painter, int blockNumber, const QColor &color,
                                 qreal docHeight, qreal blockHeight, bool hasScrollbars,
                                 bool fullLength = true);
    QList<int> getHighlightedBlockNumbers() const;

    bool lineMarkerUsed;
    bool lineMarkerVisible() const;
    int lineMarkerAreaWidth() const;
    int lineMarkerAreaOffset() const;

    void highlightMatchingBraces(QList<QTextEdit::ExtraSelection> &extraSelections);
    int findMatchingSymbol(int lookFor, int match, const QTextBlock &block, int pos, bool forward);

    QString textUnderCursor() const;

    QCompleter *completer;
    void completerHandleKeyPress(QKeyEvent * event);

    QList<CodeCollapsePosition*> *collapsePositions;
    CodeCollapsePosition findCollapsePosition(const QTextBlock &block);
    void highlightCollapsibleRegion(const QPoint &pos);
    void highlightCollapsibleRegion(const QTextBlock &block);
    void selectBlocks(QTextCursor &cur, const QTextBlock &startBlock, const QTextBlock &endBlock);
    void collapseOrExpandBlocks(QTextCursor &cur);
    void showTooltipForCollapsedRange(const QTextBlock &block, int endBlockNumber);
    int collapsedRangeCount() const;

    void showTooltipForCurrentToken(QTextCursor &cur);

    void displayCurrentTokenAsClickable(QTextCursor &cur);
    void setClickablePositions(const QTextCursor &cursor);
    void clearClickablePositions();

    bool extendSelection(QTextCursor &cur);
    void expandAllBlocks(const QTextCursor &cur);
    void expandAllBlocks(int startBlock, int endBlock);
    void expandAllBlocks(QKeyEvent *event);
    void expandBlock(const QTextBlock &block);

    void applyCurrentFontToAllEditors();
    void saveFontSettings();

    void controlKeyPressed();
    void controlKeyReleased();
    bool controlKeyDown;

    ToolTipWidget *toolTipWidget;
    void showToolTip(const QString &text, const QPoint &point, const QRect &activeRect);

    bool blockEventChanges;
    int lastParseId;
    PlSqlTreeBuilder *treeBuilder;

    bool plsqlMode;

    CodeEditor *pairEditor;
};

#endif // CODEEDITOR_H

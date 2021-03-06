#ifndef CENTEREDCHECKBOXWIDGET_H
#define CENTEREDCHECKBOXWIDGET_H

#include <QWidget>

class QCheckBox;

class CenteredCheckBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CenteredCheckBoxWidget(QWidget *parent = 0);

    QCheckBox *checkBox() const;

    //virtual bool eventFilter ( QObject * watched, QEvent * event );

private:
    QCheckBox *checkbox;

};

#endif // CENTEREDCHECKBOXWIDGET_H

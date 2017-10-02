#ifndef ScreenBlackOut_H
#define ScreenBlackOut_H

#include <QMainWindow>

class QPropertyAnimation;
class QPushButton;
class QGridLayout;
class QMouseEvent;

class ScreenBlackOut : public QMainWindow
{
    Q_OBJECT

public:
    ScreenBlackOut(QWidget *parent = 0);
    virtual ~ScreenBlackOut();
    int getsw();
    int getsh();

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    enum MovementState {
        Horizontal,
        Vertical,
        Both,
        Idle
    };
    MovementState ms;
    bool isFull;
    int sw;
    int sh;
    int m_MouseX;
    int m_MouseY;    
    int m_WidgetWidth;
    int m_WidgetHeight;
    int panelwidth;
    QGridLayout *layout;
    
    QPushButton *upUp;
    QPushButton *upDown;
    QPushButton *downUp;
    QPushButton *downDown;
    
    QPushButton *leftLeft;
    QPushButton *leftRight;
    QPushButton *rightLeft;
    QPushButton *rightRight;
    
    QPushButton *down;
    QPushButton *up;
    QPushButton *left;
    QPushButton *right;
    QPushButton *whole;
    QPushButton *cl;
    QPropertyAnimation *anime;

    
private slots:
    void downAnimation();
    void upAnimation();
    void leftAnimation();
    void rightAnimation();
    void wholeAnimation();

    
    void upUpSlot();
    void upDownSlot();
    
    void downUpSlot();
    void downDownSlot();
  
    void leftLeftSlot();
    void leftRightSlot();
    void rightRightSlot();
    void rightLeftSlot();

};

#endif // ScreenBlackOut_H

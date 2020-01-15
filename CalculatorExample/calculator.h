#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private:
    Ui::Calculator *ui;

    enum : uint8_t {
        STATE_ERROR,
        STATE_END,
        STATE_NUMBER,
        STATE_OP_HIGH,
        STATE_OP_LOW,
        STATE_EXPR
    };

    enum : uint8_t {
        CHAR_ERROR,
        CHAR_NONE,
        CHAR_NUMBER,
        CHAR_DOT,
        CHAR_OP_HIGH,
        CHAR_OP_LOW,
        CHAR_OPEN,
        CHAR_CLOSE,
    };

    uint8_t classify(ushort character);
    QPair<double, bool> evaluate(const QString &formula, int &i);

private slots:
    void onNumPressed();
    void onOperatorPressed();
    void onDotPressed();
    void onParenthesisPressed();
    void onBackspacePressed();
    void onClearPressed();
    void onEqualsPressed();
};
#endif // CALCULATOR_H

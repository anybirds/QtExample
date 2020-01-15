#include "calculator.h"
#include "ui_calculator.h"

#include <QDebug>
#include <QPair>
#include <QStack>

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);

    QPushButton *numButtons[10];
    for (int i=0; i<10; i++) {
        numButtons[i] = findChild<QPushButton *>("Button" + QString::number(i));
        connect(numButtons[i], SIGNAL(released()), this, SLOT(onNumPressed()));
    }

    connect(findChild<QPushButton *>("Add"), SIGNAL(released()), this, SLOT(onOperatorPressed()));
    connect(findChild<QPushButton *>("Subtract"), SIGNAL(released()), this, SLOT(onOperatorPressed()));
    connect(findChild<QPushButton *>("Multiply"), SIGNAL(released()), this, SLOT(onOperatorPressed()));
    connect(findChild<QPushButton *>("Divide"), SIGNAL(released()), this, SLOT(onOperatorPressed()));

    connect(findChild<QPushButton *>("Dot"), SIGNAL(released()), this, SLOT(onDotPressed()));

    connect(findChild<QPushButton *>("LeftParenthesis"), SIGNAL(released()), this, SLOT(onParenthesisPressed()));
    connect(findChild<QPushButton *>("RightParenthesis"), SIGNAL(released()), this, SLOT(onParenthesisPressed()));

    connect(findChild<QPushButton *>("Backspace"), SIGNAL(released()), this, SLOT(onBackspacePressed()));

    connect(findChild<QPushButton *>("Clear"), SIGNAL(released()), this, SLOT(onClearPressed()));

    connect(findChild<QPushButton *>("Equals"), SIGNAL(released()), this, SLOT(onEqualsPressed()));
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::onNumPressed() {
    QPushButton *button = (QPushButton *)sender();
    ui->lineEdit->setText(ui->lineEdit->text() + button->text());
}

void Calculator::onOperatorPressed() {
    QPushButton *button = (QPushButton *)sender();
    ui->lineEdit->setText(ui->lineEdit->text() + button->text());
}

void Calculator::onDotPressed() {
    QPushButton *button = (QPushButton *)sender();
    ui->lineEdit->setText(ui->lineEdit->text() + button->text());
}

void Calculator::onParenthesisPressed() {
    QPushButton *button = (QPushButton *)sender();
    ui->lineEdit->setText(ui->lineEdit->text() + button->text());
}

void Calculator::onBackspacePressed() {
    QString str = ui->lineEdit->text();
    str.chop(1);
    ui->lineEdit->setText(str);
}

void Calculator::onClearPressed() {
    ui->lineEdit->clear();
}

void Calculator::onEqualsPressed() {
    QString formula = ui->lineEdit->text();
    int i = 0;
    QPair<double, bool> value = evaluate(formula, i);
    if (value.second) {
       ui->lineEdit->setText(QString::number(value.first));
    }
}

uint8_t Calculator::classify(ushort character) {
    switch(character) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return CHAR_NUMBER;
    case '.':
        return CHAR_DOT;
    case '*':
    case '/':
        return CHAR_OP_HIGH;
    case '+':
    case '-':
        return CHAR_OP_LOW;
    case '(':
        return CHAR_OPEN;
    case ')':
        return CHAR_CLOSE;
    default:
        return CHAR_ERROR;
    }
}

QPair<double, bool> Calculator::evaluate(const QString &formula, int &i) {
    uint8_t state[8][8] = {
        {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
        {STATE_ERROR, STATE_END, STATE_NUMBER, STATE_ERROR, STATE_ERROR, STATE_OP_LOW, STATE_EXPR, STATE_END},
        {STATE_ERROR, STATE_END, STATE_NUMBER, STATE_NUMBER, STATE_OP_HIGH, STATE_OP_LOW, STATE_ERROR, STATE_END},
        {STATE_ERROR, STATE_ERROR, STATE_NUMBER, STATE_NUMBER, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
        {STATE_ERROR, STATE_ERROR, STATE_NUMBER, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_EXPR, STATE_ERROR},
        {STATE_ERROR, STATE_ERROR, STATE_NUMBER, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_EXPR, STATE_ERROR},
        {STATE_ERROR, STATE_ERROR, STATE_NUMBER, STATE_ERROR, STATE_ERROR, STATE_OP_LOW, STATE_EXPR, STATE_ERROR},
        {STATE_ERROR, STATE_END, STATE_ERROR, STATE_ERROR, STATE_OP_HIGH, STATE_OP_LOW, STATE_ERROR, STATE_END},
    };

    double num = 0.0;
    double decimal = 0.0;
    ushort op_high = 0;
    double op_high_value = 0.0;
    ushort op_low = 0;
    double op_low_value = 0.0;
    QPair<double, bool> eval;

    ushort init;
    if (i == 0) {
        init = CHAR_NONE;
    } else {
        init = CHAR_OPEN;
    }

    do {
        uint8_t prev, curr;
        ushort u;
        double value;
        if (i == 0) {
            prev = CHAR_NONE;
        } else {
            prev = classify(formula[i-1].unicode());
        }
        if (i == formula.length()) {
            curr = CHAR_NONE;
        } else {
            u = formula[i].unicode();
            curr = classify(u);
            value = double(u - '0');
        }

        switch(state[prev][curr]) {
        case STATE_ERROR:
            return qMakePair(0, false);
        case STATE_END:
            if (!((init == CHAR_NONE && curr == CHAR_NONE) || (init == CHAR_OPEN && curr == CHAR_CLOSE))) {
                return qMakePair(0, false);
            }
            if (op_high == '*') {
                num *= op_high_value;
            } else if (op_high == '/') {
                num = op_high_value / num;
            }
            if (op_low == '+') {
                num += op_low_value;
            } else if (op_low == '-') {
                num = op_low_value - num;
            }
            return qMakePair(num, true);
        case STATE_NUMBER:
            if (u == '.') {
                if (decimal != 0.0) {
                    return qMakePair(0, false);
                }
                decimal = 1.0;
            } else if (decimal != 0.0) {
                decimal /= 10.0;
                num += decimal * value;
            } else {
                num *= 10.0;
                num += value;
            }
            break;
        case STATE_OP_HIGH:
            if (op_high == '*') {
                op_high_value *= num;
            } else if (op_high == '/') {
                op_high_value /= num;
            } else {
                op_high_value = num;
            }
            num = 0.0;
            decimal = 0.0;
            op_high = u;
            break;
        case STATE_OP_LOW:
            if (op_high == '*') {
                num *= op_high_value;
            } else if (op_high == '/') {
                num = op_high_value / num;
            }
            if (op_low == '+') {
                op_low_value += num;
            } else if (op_low == '-') {
                op_low_value -= num;
            } else {
                op_low_value = num;
            }
            num = 0.0;
            decimal = 0.0;
            op_high = 0;
            op_high_value = 0.0;
            op_low = u;
            break;
        case STATE_EXPR:
            eval = evaluate(formula, ++i);
            if (!eval.second) {
                return eval;
            }
            num = eval.first;
            if (op_high == '*') {
                num *= op_high_value;
            } else if (op_high == '/') {
                num = op_high_value / num;
            }
            op_high = 0;
            op_high_value = 0.0;
            break;
        default:
            break;
        }
    } while (++i);
}


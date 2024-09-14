#include "dialog.hpp"
#include "./ui_dialog.h"
#include <QClipboard>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
    , m_replaceRe{"[- :]"}
    , m_squeezeRe{"(?<=([-: ]))\\1"}
    , m_clearRe{"[,'.\"!?;<>()]"}
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);

    setWindowTitle("ZMK Antecedent Morph Macro Generator");

    connect(ui->closeButton, &QPushButton::clicked, []{
        QApplication::quit();
    });
    connect(ui->clearButton, &QPushButton::clicked, [this]{
        ui->phraseEdit->clear();
    });
    connect(ui->copyButton, &QPushButton::clicked, [this]{
        QApplication::clipboard()->setText(ui->macroText->toPlainText());
    });

    connect(ui->phraseEdit, &QLineEdit::textChanged, this, &Dialog::generateMacro);
    connect(ui->antecedent, &QComboBox::currentIndexChanged, this, &Dialog::generateMacro);
    connect(ui->schema, &QComboBox::currentIndexChanged, this, &Dialog::generateMacro);

    m_template = QString(R"TMPL(ZMK_MACRO(%1_%2,
            wait-ms = <U_ANTMORPH_MACRO_WAIT>;
            tap-ms = <U_ANTMORPH_MACRO_TAP>;
            // %3%4
            bindings = <%5>;
        ))TMPL");

    ui->templateText->setPlainText(m_template);

    m_map["a"] = "A";
    m_map["b"] = "B";
    m_map["c"] = "C";
    m_map["d"] = "D";
    m_map["e"] = "E";
    m_map["f"] = "F";
    m_map["g"] = "G";
    m_map["h"] = "H";
    m_map["i"] = "I";
    m_map["j"] = "J";
    m_map["k"] = "K";
    m_map["l"] = "L";
    m_map["m"] = "M";
    m_map["n"] = "N";
    m_map["o"] = "O";
    m_map["p"] = "P";
    m_map["q"] = "Q";
    m_map["r"] = "R";
    m_map["s"] = "S";
    m_map["t"] = "T";
    m_map["u"] = "U";
    m_map["v"] = "V";
    m_map["w"] = "W";
    m_map["x"] = "X";
    m_map["y"] = "Y";
    m_map["z"] = "Z";

    m_map["A"] = "LS(A)";
    m_map["B"] = "LS(B)";
    m_map["C"] = "LS(C)";
    m_map["D"] = "LS(D)";
    m_map["E"] = "LS(E)";
    m_map["F"] = "LS(F)";
    m_map["G"] = "LS(G)";
    m_map["H"] = "LS(H)";
    m_map["I"] = "LS(I)";
    m_map["J"] = "LS(J)";
    m_map["K"] = "LS(K)";
    m_map["L"] = "LS(L)";
    m_map["M"] = "LS(M)";
    m_map["N"] = "LS(N)";
    m_map["O"] = "LS(O)";
    m_map["P"] = "LS(P)";
    m_map["Q"] = "LS(Q)";
    m_map["R"] = "LS(R)";
    m_map["S"] = "LS(S)";
    m_map["T"] = "LS(T)";
    m_map["U"] = "LS(U)";
    m_map["V"] = "LS(V)";
    m_map["W"] = "LS(W)";
    m_map["X"] = "LS(X)";
    m_map["Y"] = "LS(Y)";
    m_map["Z"] = "LS(Z)";

    m_map["0"] = "N0";
    m_map["1"] = "N1";
    m_map["2"] = "N2";
    m_map["3"] = "N3";
    m_map["4"] = "N4";
    m_map["5"] = "N5";
    m_map["6"] = "N6";
    m_map["7"] = "N7";
    m_map["8"] = "N8";
    m_map["9"] = "N9";

    m_map[" "] = "SPACE";
    m_map["!"] = "EXCL";
    m_map["@"] = "AT";
    m_map["#"] = "HASH";
    m_map["$"] = "DLLR";
    m_map["%"] = "PRCNT";
    m_map["^"] = "CARET";
    m_map["&"] = "AMPS";
    m_map["*"] = "STAR";
    m_map["("] = "LPAR";
    m_map[")"] = "RPAR";
    m_map["="] = "EQUAL";
    m_map["+"] = "PLUS";
    m_map["-"] = "MINUS";
    m_map["_"] = "UNDER";
    m_map["/"] = "FSLH";
    m_map["?"] = "QMARK";
    m_map["\\"] = "BSLH";
    m_map["|"] = "PIPE";
    m_map[";"] = "SEMI";
    m_map[":"] = "COLON";
    m_map["'"] = "APOS";
    m_map["\""] = "DQT";
    m_map[","] = "COMMA";
    m_map["."] = "DOT";
    m_map[">"] = "GT";
    m_map["<"] = "LT";
    m_map["["] = "LBKT";
    m_map["]"] = "RBKT";
    m_map["{"] = "LBRC";
    m_map["}"] = "RBRC";
    m_map["`"] = "GRAVE";
    m_map["~"] = "TILDE";

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::generateMacro()
{
    auto tmpl = ui->templateText->toPlainText();
    auto phrase = ui->phraseEdit->text();
    if (phrase.isEmpty()) {
        ui->macroText->setPlainText("Enter the phrase");
        return;
    }

    auto ident = phrase;
    ident.replace(m_squeezeRe, "");
    ident.replace(m_clearRe, "");
    ident.replace(m_replaceRe, "_");

    QString prefix, firstOp;
    if (ui->antecedent->currentText().toLower() == phrase.first(1).toLower()) {
        prefix = QString("(") + phrase.first(1).toLower() + ")";
        firstOp = "";
        phrase = phrase.sliced(1);
    } else {
        prefix = QString("[") + ui->antecedent->currentText().toLower() + "]";
        firstOp = "&kp BSPC ";
    }
    if (ident.isEmpty()) {
        ui->macroText->setPlainText("Enter the phrase");
        return;
    }
    QString bindings;
    for (auto l: phrase.split(QString())) {
        if (l.isEmpty())
            continue;

        if (m_map.contains(l)) {
            bindings += QString("&kp ") + m_map.value(l) + " ";
        } else {
            ui->macroText->setPlainText("Cannot generate macro for this phrase");
            return;
        }
    }
    ui->macroText->setPlainText(
        tmpl.arg(
            schemaPrefix(),                // 1
            ident.toLower(),               // 2
            prefix,                        // 3
            phrase.toLower(),              // 4
            firstOp + bindings.trimmed()   // 5
        )
    );
}

QString Dialog::schemaPrefix() const
{
    switch (ui->schema->currentIndex()) {
    case English:
        return "amen";
    case Cpp:
        return "amcpp";
    case CMake:
        return "amcmk";
    }

    return QString{};
}

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <qDebug>
#include <QTextStream>
#include <QFile>
#include <QLabel>
#include <QMovie>
#include <QVector>
#include <QClipboard>
#include <QListWidget>

class Generator{
public:
    virtual QString generuj() = 0;
    virtual ~Generator() = default;
};


class generator_hasel : public Generator{
protected:
    int dlugosc;

public:

    generator_hasel(){
        dlugosc = 12;
    }
    generator_hasel(int x){
       dlugosc = x;
    }
    virtual ~generator_hasel(){}


    QString generuj() override
    {
        QString znaki =
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        QString haslo;

        for (int i = 0; i < dlugosc; i++)
        {
            int index = QRandomGenerator::global()->bounded(znaki.length());
            haslo += znaki[index];
        }
        return haslo;
    }
    class Info{
    public:
        static void pokaz(){
            qDebug()<< "Generator hasel aktywny!";

        }
    };
};



class lepszy_generator : public generator_hasel{
public:
    lepszy_generator(int x) : generator_hasel(x){}

    QString generuj() override{
        QString znaki =
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "1234567890"
            "!@#$%^&*()";

        QString haslo;

        for (int i = 0; i < dlugosc; i++){
            int index = QRandomGenerator::global()->bounded(znaki.length());
            haslo += znaki[index];
        }
        return haslo;
    }
};


void zapisz(QString haslo){
    QFile file("hasła.txt");

    if(!file.open(QIODevice::Append | QIODevice::Text)){
        qDebug()<< "Blad otwarcia pliku!";
        return;
    }

    QTextStream out(&file);

    out << haslo << "\n";

    file.close();
}


template<typename T>
void wypisz(T wartosc){
    qDebug() << wartosc;
}


class HasloRepo{
public:
    QVector<QString> historia;

    void dodaj(const QString& x){
        historia.push_back(x);
    }

    QVector<QString> pobierz(){
        return historia;
    }
};





int main(int argc, char *argv[])
{

    bool historia_wyswietlona = false;
    HasloRepo repo;


    generator_hasel::Info::pokaz();


    QApplication app(argc, argv);

    QWidget okno;
    okno.setWindowTitle("Generator Haseł");

    QVBoxLayout *layout = new QVBoxLayout();

    QLineEdit *pole_hasla = new QLineEdit();
    pole_hasla->setReadOnly(true);

    QCheckBox *checkbox_special = new QCheckBox("Znaki specjalne + cyfry");
    QPushButton *kopiuj = new QPushButton("Kopiuj hasło do schowka");
    QPushButton *przycisk = new QPushButton("Generuj haslo");
    QListWidget *historia = new QListWidget();

    QLabel *gifLabel = new QLabel();
    QMovie *movie = new QMovie("gif.gif");
    if (!movie->isValid()){
        qDebug() << "Nie można zaladowac gif!";
    }
    gifLabel->setMovie(movie);
    movie->start();


    layout->addWidget(gifLabel);
    layout->addWidget(pole_hasla);
    layout->addWidget(checkbox_special);
    layout->addWidget(kopiuj);
    layout->addWidget(przycisk);
    layout->addWidget(historia);


    QObject::connect(przycisk, &QPushButton::clicked, [&](){
        QString haslo;


        if(checkbox_special->isChecked()){
            lepszy_generator gen(20);
            haslo = gen.generuj();
        }
        else{
            generator_hasel gen(12);
            haslo = gen.generuj();
        }
        pole_hasla->setText(haslo);
        zapisz(haslo);
        repo.dodaj(haslo);

        historia->addItem(haslo);
    });

    QObject::connect(kopiuj, &QPushButton::clicked, [&](){

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(pole_hasla->text());

    kopiuj->setText("Skopiowano hasło!");
    });


    //STYLE
    okno.setLayout(layout);
    okno.setStyleSheet(
        "background-color: black;"
        );
    pole_hasla->setStyleSheet(
        "background-color: white;"
        "color: black;"
        "border: 2px solid green;"
        "border-radius: 10px;"
        "padding: 6px;"
        "font-size: 15px;"
        );
    kopiuj->setStyleSheet(
        "background-color: green;"
        "color: white;"
        "border: 2px solid black;"
        "padding: 3px;"
        "font-size: 15px;"
        );
    przycisk->setStyleSheet(
        "background-color: green;"
        "color: white;"
        "border: 2px solid black;"
        "padding: 6px;"
        "font-size: 15px;"
        );
   checkbox_special->setStyleSheet(
        "color: white; ");
   historia->setStyleSheet(
        "color: white; ");

    okno.setWindowIcon(QIcon("icon.png"));
    okno.resize(300, 300);
    okno.show();
    return app.exec();
}
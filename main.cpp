#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QtWebKitWidgets/QWebView>
#include <QUrl>

// IMPORTANT: These headers fix the "SocketState/SslError" errors you saw
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QSslError>

class LiteBrowser : public QMainWindow {
    Q_OBJECT
public:
    LiteBrowser() {
        setWindowTitle("OpenSource Lite Browser");
        resize(1200, 800);

        // UI Setup
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
        QHBoxLayout *navLayout = new QHBoxLayout();

        addressBar = new QLineEdit();
        QPushButton *newTabBtn = new QPushButton("+ New Tab");
        
        navLayout->addWidget(addressBar);
        navLayout->addWidget(newTabBtn);

        tabs = new QTabWidget();
        tabs->setTabsClosable(true);

        mainLayout->addLayout(navLayout);
        mainLayout->addWidget(tabs);
        setCentralWidget(centralWidget);

        // Logic
        connect(newTabBtn, &QPushButton::clicked, this, &LiteBrowser::addNewTab);
        connect(addressBar, &QLineEdit::returnPressed, this, &LiteBrowser::loadUrl);
        connect(tabs, &QTabWidget::tabCloseRequested, tabs, &QTabWidget::removeTab);
        
        // Open the first tab
        addNewTab();
    }

private slots:
    void addNewTab() {
        QWebView *web = new QWebView();
        web->load(QUrl("https://www.google.com"));
        
        int index = tabs->addTab(web, "New Tab");
        tabs->setCurrentIndex(index);

        // Update tab title when page loads
        connect(web, &QWebView::titleChanged, [=](const QString &title){
            tabs->setTabText(tabs->indexOf(web), title.left(15) + "...");
        });
    }

    void loadUrl() {
        QString urlText = addressBar->text();
        if (!urlText.startsWith("http")) urlText.prepend("https://");
        
        QWebView *currentWeb = qobject_cast<QWebView*>(tabs->currentWidget());
        if (currentWeb) {
            currentWeb->load(QUrl(urlText));
        }
    }

private:
    QTabWidget *tabs;
    QLineEdit *addressBar;
};

#include "main.moc" // Required if putting everything in one .cpp file

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    LiteBrowser browser;
    browser.show();
    return app.exec();
}

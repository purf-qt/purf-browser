#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QUrl>
#include <QKeyEvent>

// IMPORTANT: These headers fix the "SocketState/SslError" errors
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
        QWebEngineView *web = new QWebEngineView();
        
        // FIX: Enable CSS and all web features
        QWebEngineSettings *settings = web->settings();
        settings->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
        settings->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
        settings->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
        settings->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
        settings->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
        settings->setAttribute(QWebEngineSettings::XSSAuditingEnabled, false);
        
        web->load(QUrl("https://www.google.com"));
        
        // IMPORTANT FIX: Set focus policy so keyboard input works
        web->setFocusPolicy(Qt::StrongFocus);
        
        int index = tabs->addTab(web, "New Tab");
        tabs->setCurrentIndex(index);

        // Update tab title when page loads
        connect(web, &QWebEngineView::titleChanged, [=](const QString &title){
            tabs->setTabText(tabs->indexOf(web), title.left(15) + "...");
        });
        
        // Give focus to the web view when tab is shown
        web->setFocus();
    }

    void loadUrl() {
        QString urlText = addressBar->text();
        if (!urlText.startsWith("http")) urlText.prepend("https://");
        
        QWebEngineView *currentWeb = qobject_cast<QWebEngineView*>(tabs->currentWidget());
        if (currentWeb) {
            currentWeb->load(QUrl(urlText));
            // Set focus to web view after loading
            currentWeb->setFocus();
        }
    }

private:
    QTabWidget *tabs;
    QLineEdit *addressBar;
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    LiteBrowser browser;
    browser.show();
    return app.exec();
}

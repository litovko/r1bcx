#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <c_audio.h>

int main(int argc, char *argv[])
{
    qmlRegisterType<c_audio>("HYCO", 1, 0, "CAudio");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

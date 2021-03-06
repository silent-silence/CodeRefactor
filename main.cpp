#ifdef ENV_TEST

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#else

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Interface/Interface.h"

int main(int argc, char **argv)
{
   QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
   QCoreApplication::setOrganizationName("silent-silence");
   QCoreApplication::setOrganizationDomain("CodeRefactor");

   QGuiApplication app(argc, argv);
   QQmlApplicationEngine engine;

   const QUrl url(QStringLiteral("qrc:/main.qml"));
   qmlRegisterType<Interface>("CodeInterface", 1, 0, "Refactor");

   QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                    &app, [url](QObject *obj, const QUrl &objUrl) {
       if (!obj && url == objUrl)
           QCoreApplication::exit(-1);
   }, Qt::QueuedConnection);
   engine.load(url);

   return app.exec();
}
#endif

#include "authentication.h"

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    QApplication app(argc, argv);

    DatabaseManager dbManager("movies.db");
    Authentication login(dbManager);

    login.exec();

    return app.exec();
}

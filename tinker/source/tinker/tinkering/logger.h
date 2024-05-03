// I WANT SOMETHING SOMETHING LIKE QT HAS:

// void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
// {
//     QByteArray localMsg = msg.toLocal8Bit();
//     const char *file = context.file ? context.file : "";
//     const char *function = context.function ? context.function : "";
//     switch (type) {
//     case QtDebugMsg:
//         fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//         break;
//     case QtInfoMsg:
//         fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//         break;
//     case QtWarningMsg:
//         fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//         break;
//     case QtCriticalMsg:
//         fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//         break;
//     case QtFatalMsg:
//         fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//         break;
//     }
// }


// AND SPDLOG:

// trace = SPDLOG_LEVEL_TRACE,
// debug = SPDLOG_LEVEL_DEBUG,
// info = SPDLOG_LEVEL_INFO,
// warn = SPDLOG_LEVEL_WARN,
// err = SPDLOG_LEVEL_ERROR,
// critical = SPDLOG_LEVEL_CRITICAL,
// off = SPDLOG_LEVEL_OFF,
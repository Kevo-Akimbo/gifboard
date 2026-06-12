#pragma once

#include <QImageReader>
#include <QString>

inline QSize getImageSize(const QString &filepath) {
  auto reader = QImageReader(filepath);
  if (reader.format().isEmpty()) {
    return QSize();
  } else {
    return reader.size();
  }
}

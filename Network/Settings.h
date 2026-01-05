#pragma once

#include "settings.h"

#include <QtNetwork/QNetworkProxy>

Q_DECLARE_METATYPE(QNetworkProxy::ProxyType)

namespace Quotient {
class QUOTIENT_API NetworkSettings : public SettingsGroup {
    Q_OBJECT
    QUO_DECLARE_SETTING(QNetworkProxy::ProxyType, proxyType, setProxyType)
    QUO_DECLARE_SETTING(QString, proxyHostName, setProxyHostName)
    QUO_DECLARE_SETTING(quint16, proxyPort, setProxyPort)
    Q_PROPERTY(QString proxyHost READ proxyHostName WRITE setProxyHostName)
public:
    explicit NetworkSettings() : SettingsGroup(u"Network"_s) {}

    Q_INVOKABLE void setupApplicationProxy() const;
};
} // namespace Quotient

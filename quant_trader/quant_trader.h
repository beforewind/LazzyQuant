#ifndef QUANT_TRADER_H
#define QUANT_TRADER_H

#include <boost/optional.hpp>

#include "market_watcher_interface.h"
#include "trade_executer_interface.h"

template <typename T> class QList;
template <typename Key, typename T> class QMap;
template <typename Key, typename T> class QMultiMap;

class Bar;
class BarCollector;
class AbstractIndicator;
class AbstractStrategy;

class QuantTrader : public QObject
{
    Q_OBJECT
protected:
    com::lazzyquant::market_watcher *pWatcher;
    com::lazzyquant::trade_executer *pExecuter;

    // Following QString keys are instumentIDs
    QMap<QString, BarCollector*> collector_map;
    QMap<QString, QMap<int, QList<Bar>>> bars_map;
    QMultiMap<QString, AbstractIndicator*> indicator_map;
    QMultiMap<QString, AbstractStrategy*> strategy_map;
    QMap<QString, boost::optional<int>> position_map;

    QString kt_export_dir;
    void loadQuantTraderSettings();
    void loadTradeStrategySettings();
    QList<Bar>* getBars(const QString &instrumentID, const QString &time_frame_str);

    QTimer *saveBarTimer;
    QList<QTime> saveBarTimePoints;
    QList<QList<BarCollector*>> collectorsToSave;
    int saveBarTimeIndex;

public:
    explicit QuantTrader(QObject *parent = 0);
    ~QuantTrader();

    static QuantTrader *instance;
    AbstractIndicator* registerIndicator(const QString &instrumentID, const QString &time_frame_str, QString indicator_name, ...);

signals:

private slots:
    void onMarketData(const QString& instrumentID, uint time, double lastPrice, int volume);
    void onNewBar(const QString &instrumentID, int time_frame, const Bar& bar);
    void saveBarsAndResetTimer();
};

#endif // QUANT_TRADER_H

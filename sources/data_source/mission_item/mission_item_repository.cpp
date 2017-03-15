#include "mission_item_repository.h"

// Qt
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

// Internal
#include "mission_item.h"

using namespace data_source;

class MissionItemRepository::Impl
{
public:
    QSqlQuery query;

    bool runQuerry()
    {
        if (query.exec()) return true;
        qWarning() << query.lastError() << query.executedQuery();
        return false;
    }
};

MissionItemRepository::MissionItemRepository():
    d(new Impl())
{}

MissionItemRepository::~MissionItemRepository()
{
    delete d;
}

bool MissionItemRepository::createRepository()
{
    d->query.prepare("CREATE TABLE mission_items ("
                     "id INTEGER PRIMARY KEY NOT NULL,"
                     "sequence INTEGER,"
                     "command INTEGER,"
                     "altitude REAL,"
                     "altitude_relative BOOLEAN,"
                     "latitude DOUBLE,"
                     "longitude DOUBLE,"
                     "radius REAL,"
                     "pitch REAL,"
                     "periods INTEGER)");

    return d->runQuerry();
}

bool MissionItemRepository::dropRepository()
{
    d->query.prepare("DROP TABLE mission_items");
    return d->runQuerry();
}

MissionItem* MissionItemRepository::createMissionItem()
{
    d->query.prepare("INSERT INTO mission_items ("
                     "sequence, command, altitude, altitude_relative, "
                     "latitude, longitude, radius, pitch, periods) "
                     "VALUES (:sequence, :command, :altitude, :altitude_relative, "
                     ":latitude, :longitude, :radius, :pitch, :periods)");

    d->query.bindValue(":sequence", -1);
    d->query.bindValue(":command", 0);
    d->query.bindValue(":altitude", 0);
    d->query.bindValue(":altitude_relative", 0);
    d->query.bindValue(":latitude", 0);
    d->query.bindValue(":longitude", 0);
    d->query.bindValue(":radius", 0);
    d->query.bindValue(":pitch", 0);
    d->query.bindValue(":periods", 0);

    if (d->runQuerry()) return new MissionItem(d->query.lastInsertId().toInt());
    return nullptr;
}

MissionItem* MissionItemRepository::readMissionItem(int id)
{
    d->query.prepare("SELECT * FROM mission_items WHERE id = :id");
    d->query.bindValue(":id", id);

    if (d->runQuerry() && d->query.next())
    {
        auto item = new MissionItem(id);

        item->setSequence(d->query.value("sequence").toInt());
        item->setCommand(Command(d->query.value("command").toInt()));
        item->setAltitude(d->query.value("altitude").toFloat());
        item->setAltitudeRelative(d->query.value("altitude_relative").toBool());
        item->setLatitude(d->query.value("latitude").toDouble());
        item->setLongitude(d->query.value("longitude").toDouble());
        item->setRadius(d->query.value("radius").toFloat());
        item->setPitch(d->query.value("pitch").toFloat());
        item->setPeriods(d->query.value("periods").toInt());

        return item;
    }
    return nullptr;
}

bool MissionItemRepository::updateMissionItem(MissionItem* item)
{
    d->query.prepare("UPDATE mission_items SET "
                     "sequence = :sequence,"
                     "command = :command,"
                     "altitude = :altitude,"
                     "altitude_relative = :altitude_relative,"
                     "latitude = :latitude,"
                     "longitude = :longitude,"
                     "radius = :radius,"
                     "pitch = :pitch,"
                     "periods = :periods "
                     "WHERE id = :id");

    d->query.bindValue(":id", item->id());
    d->query.bindValue(":sequence", item->sequence());
    d->query.bindValue(":command", int(item->command()));
    d->query.bindValue(":altitude", item->altitude());
    d->query.bindValue(":altitude_relative", item->isAltitudeRelative());
    d->query.bindValue(":latitude", item->latitude());
    d->query.bindValue(":longitude", item->longitude());
    d->query.bindValue(":radius", item->radius());
    d->query.bindValue(":pitch", item->pitch());
    d->query.bindValue(":periods", item->periods());

    return d->runQuerry();
}

bool MissionItemRepository::deleteMissionItem(MissionItem* item)
{
    d->query.prepare("DELETE FROM mission_items WHERE id = :id");
    d->query.bindValue(":id", item->id());
    return d->runQuerry();
}

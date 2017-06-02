import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtCharts 2.0
import JAGCS 1.0

import "qrc:/Controls" as Controls
import "qrc:/Indicators" as Indicators

AbstractVehicleView {
    id: root

    signal maximize()

    contentWidth: row.implicitWidth
    contentHeight: row.implicitHeight

    RowLayout {
        id: row
        width: parent.width

        Indicators.ArtificialHorizon {
            available: root.online
            armed: root.armed
            pitch: root.pitch
            roll: root.roll
            pitchScaleEnabled: false
            markWidth: 2
            Layout.minimumWidth: palette.controlBaseSize * 2
        }

        Indicators.Compass {
            id: compass
            tickFactor: 15
            scalesRatio: 0.11
            course: root.course
            heading: root.heading
            Layout.minimumWidth: palette.controlBaseSize * 3
        }

        ColumnLayout {

            Controls.Label {
                text: qsTr("MAV ID") + ": " + mavId
                Layout.fillWidth: true
            }

            Controls.Label {
                text: qsTr("MAV") + ": " + name
            }

            Controls.Label {
                text: qsTr("Mode") + ": " + modeString
            }
        }
    }

    Controls.Button {
        anchors.top: parent.top
        anchors.right: parent.right
        iconSource: "qrc:/ui/plus.svg"
        onClicked: maximize()
    }
}

import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQml.Models 2.12

Page {
    ListView {
        id: listView
        width: parent.width
        height: parent.height
        spacing: 2
        model: ObjectModel {
            CtrSlider { title: qsTr("AIL"); fact: m.rc_roll; width: listView.width }
            CtrSlider { title: qsTr("ELV"); fact: m.rc_pitch; width: listView.width }
            CtrSlider { title: qsTr("THR"); fact: m.rc_throttle; from: 0; width: listView.width }
            CtrSlider { title: qsTr("YAW"); fact: m.rc_yaw; width: listView.width }
            CtrSlider { title: qsTr("FLP"); fact: m.ctr_flaps; from: 0; stepSize: 0.1; width: listView.width }
            CtrSlider { title: qsTr("BRK"); fact: m.ctr_brake; from: 0; stepSize: 0.1; width: listView.width }
            CtrSlider { title: qsTr("ABR"); fact: m.ctr_airbrk; from: 0; stepSize: 0.1; width: listView.width }
        }
    }
}

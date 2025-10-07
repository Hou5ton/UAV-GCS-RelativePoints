import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.3

import Apx.Common 1.0

import APX.Facts 1.0

ColumnLayout {
    id: control

    property alias model: listView.model
    property alias delegate: listView.delegate
    property alias actionsModel: actionsItem.model

    property alias header: listView.header
    property alias headerItem: listView.headerItem

    property ListView listView: listView

    function factButtonTriggered(fact)
    {
        if(factMenu)
            factMenu.factButtonTriggered(fact)
    }
    property int maximumHeight: ui.window.height
                                -MenuStyle.titleSize
                                -(actionsItem.visible?actionsItem.implicitHeight+spacing:0)
                                -(listView.headerItem?listView.headerItem.implicitHeight:0)
                                -(listView.footerItem?listView.footerItem.implicitHeight:0)
                                -32
    //facts
    ListView {
        id: listView
        Layout.fillHeight: true
        Layout.fillWidth: true
        implicitHeight: Math.min(maximumHeight, Math.max(contentHeight,MenuStyle.itemSize*3)+8)
    }

    //actions
    RowLayout {
        id: actionsItem

        Layout.alignment: Qt.AlignRight
        Layout.bottomMargin: control.spacing

        spacing: 5
        visible: repeater.count>0

        property alias model: repeater.model
        Repeater {
            id: repeater
            model: fact.actionsModel
            delegate: Loader{
                asynchronous: true
                active: modelData && modelData.visible && ((modelData.options&Fact.ShowDisabled)?true:modelData.enabled)
                visible: active
                sourceComponent: Component {
                    FactMenuAction {
                        fact: modelData
                        showText: true
                        onTriggered: control.factButtonTriggered(modelData)
                    }
                }
            }
        }
    }
}

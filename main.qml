import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import HYCO 1.0
import "./js/Log.js" as Logjs

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    CAudio{
        id: au
    Component.onCompleted: {
        //console.log(Logjs.serialize(availableDevices))

        //console.log("availableDebices:"+availableDevices)
    }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Page1 {
        }

        Page {
            Rectangle {
                anchors.fill: parent
                border.color: "yellow"
                border.width: 2
                color: "gray"
                ListView {
                    anchors.fill: parent
                    model: au.availableDevices
                    delegate: Button {
                        width: 300
                        text: modelData
                    }
                }
            }
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        TabButton {
            text: qsTr("First")
        }
        TabButton {
            text: qsTr("Second")
        }
    }
}

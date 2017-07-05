import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtCharts 2.0


import HYCO 1.0
import "./js/Log.js" as Logjs

ApplicationWindow {
    visible: true
    width: 800
    height: 600
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
        Page {
            Rectangle {
                anchors.fill: parent
                border.color: "yellow"
                border.width: 2
                color: "gray"
                ChartView {
                    id:chart
                    width: 600
                    height: 400
                    theme: ChartView.ChartThemeDark
                    antialiasing: true
                    anchors.centerIn: parent
                    Component.onCompleted: {
                        au.series=ls
                        au.freq=fls
                    }
                    LineSeries{
                        id: ls
                        name: "Sound"
                        color:"lightblue"
                        ValueAxis {
                                id: valueAxisX
                                min: 0
                                max: 2000
                                tickCount: 11
                                labelFormat: "%.0f"
                            }
                        ValueAxis {
                                id: valueAxisY
                                min: -1
                                max: 1
                                tickCount: 10
                                labelFormat: "%.1f"
                            }
                        axisX: valueAxisX
                        axisY: valueAxisY
                    }
                    LineSeries{
                        id: fls
                        name: "Freq"
                        style: Qt.DotLine
                        color: "lightgreen"
                        ValueAxis {
                                id: fvalueAxisX
                                min: 0
                                max: 2000
                                tickCount: 11
                                labelFormat: "%.0f"
                            }
                        ValueAxis {
                                id: fvalueAxisY
                                min: -1
                                max: 1
                                tickCount: 10
                                labelFormat: "%.1f"
                            }
                        axisX: fvalueAxisX
                        axisY: fvalueAxisY
                    }

                }
            }

        }

        Page {

            Rectangle {
                anchors.fill: parent
                border.color: "yellow"
                border.width: 2
                color: "gray"
                ListView {
                    width: 300
                    height: 100
                    anchors.right: parent.right
                    anchors.top: parent.top
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

import QtQuick          2.12
import QtLocation       5.11
import QtPositioning    5.11

Map {
    id: control


    signal moved()
    signal clicked(var coordinate)
    signal menuRequested()

    property var calibrationazdist: apx.tools.calibrationazdist
    property var sendpositionfix: apx.tools.sendpositionfix
    property var rulerPlugin: apx.tools.ruler

    readonly property var mouseCoordinate: mouseCoordinateRaw.isValid?mouseCoordinateRaw:center
    property var mouseClickCoordinate: center
    property point mouseClickPoint: Qt.point(pressX, pressY)
    readonly property real lat_default: apx.settings.interface.lat.value
    readonly property real lon_default: apx.settings.interface.lon.value
   // readonly property real zoom_default: apx.settings.interface.zoom.value

    property var defaultCoordinate: QtPositioning.coordinate(lat_default,lon_default)
   // property var defaultZoomLevel: zoom_default



    function showRegion(r)
    {
        followStop()
        resetFlicking()
        tilt=0
        bearing=0
        r.width*=1.5
        r.height*=1.5
        visibleRegion=r
    }

    function centerOn(coord) {
        resetFlicking();
        if(coord.latitude===0 || coord.longitude===0)return
        anim.stop()
        center=coord
    }


    Behavior on zoomLevel {
        enabled: ui.smooth; NumberAnimation {duration: 500; easing.type: Easing.InOutQuad; }
    }

    onMoved: {
        anim.stop()
        followStop()
        apx.settings.setDefaults(center.latitude,
                                 center.longitude,
                                 zoomLevel)
    }

    onZoomLevelChanged: {
        apx.settings.setDefaults(center.latitude,
                                 center.longitude,
                                 zoomLevel)
    }

    //smooth panning
    CoordinateAnimation {
        id: anim
        duration: ui.smooth?500:0
        direction: CoordinateAnimation.Shortest
        easing.type: Easing.InOutCubic
        target: control
        property: "center"
    }
    function showCoordinate(coord)
    {
        if(follow) return
        resetFlicking();
        if(coord.latitude===0 || coord.longitude===0)return
        if(isFarMove(center,coord)){
            anim.stop()
            centerOn(coord)
            return
        }
        anim.easing.type=Easing.InOutCubic
        if(!anim.running)anim.from=center
        anim.to=coord
        anim.start()
    }
    function move_to_coo(coord)
    {
        resetFlicking();
        if(coord.latitude===0 || coord.longitude===0){
            return
        }
        anim.stop()
        centerOn(coord)
    }
    function flickToCoordinate(coord)
    {
        if(follow) return
        resetFlicking();
        anim.easing.type=Easing.Linear
        if(!anim.running)anim.from=center
        anim.to=center.atDistanceAndAzimuth(center.distanceTo(coord)*0.2,center.azimuthTo(coord))
        anim.start()
    }
    function isFarMove(from,to,factor)
    {
        var p1=fromCoordinate(from,false);
        var p2=fromCoordinate(to,false);
        if(typeof(factor)=='undefined')factor=8
        if(Math.abs(p1.x-p2.x)>width*factor || Math.abs(p1.y-p2.y)>height*factor){
            return true
        }
        return false
    }
    function resetFlicking()
    {
        gesture.enabled=false
        gesture.enabled=true
    }

    //---------------------------
    //item follow
    function followItem(item)
    {
        if(!item)return;
        itemToFollow=item;
        follow=true;
        centerOn(item.coordinate)
    }
    function followStop()
    {
        follow=false;
    }

    //follow map circle
    Rectangle {
        z: 1000
        border.color: "#222"
        border.width: 2
        color: "transparent"
        width: 13
        height: width
        anchors.centerIn: parent
        visible: follow
        radius: width/2
    }

    //Item follow
    property bool follow: false
    property MapQuickItem itemToFollow
    Connections {
        enabled: follow
        target: itemToFollow
        onCoordinateChanged: if(follow) centerOn(itemToFollow.coordinate)
    }

    //internal
    color: '#333'

    center: defaultCoordinate

    //zoomLevel: defaultZoomLevel

    gesture.acceptedGestures: MapGestureArea.PanGesture | MapGestureArea.FlickGesture | MapGestureArea.PinchGesture | MapGestureArea.RotationGesture | MapGestureArea.TiltGesture
    gesture.enabled: true

    focus: true
    onCopyrightLinkActivated: Qt.openUrlExternally(link)



    gesture.onPanStarted:       moved()
    gesture.onFlickStarted:     moved()

    //Keyboard controls
    Keys.onPressed: {
        if (event.key === Qt.Key_Plus) {
            zoomLevel++;
        } else if (event.key === Qt.Key_Minus) {
            zoomLevel--;
        } else if (event.key === Qt.Key_Left || event.key === Qt.Key_Right ||
                   event.key === Qt.Key_Up   || event.key === Qt.Key_Down) {
            var dx = 0;
            var dy = 0;
            switch (event.key) {
                case Qt.Key_Left: dx = width / 4; break;
                case Qt.Key_Right: dx = -width / 4; break;
                case Qt.Key_Up: dy = height / 4; break;
                case Qt.Key_Down: dy = -height / 4; break;
            }
            center = toCoordinate(Qt.point(width / 2.0 - dx, height / 2.0 - dy));
        }
    }


    // Mouse
    property int lastX: -1
    property int lastY: -1
    property int pressX: -1
    property int pressY: -1
    property int jitterThreshold: 30
    property var mouseCoordinateRaw: toCoordinate(Qt.point(mouseArea.mouseX, mouseArea.mouseY))

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true
        propagateComposedEvents: true

        onPressed : {
            forceActiveFocus()
            lastX = mouse.x
            lastY = mouse.y
            pressX = mouse.x
            pressY = mouse.y
            mouseClickCoordinate = toCoordinate(Qt.point(mouse.x, mouse.y))
        }

        onPositionChanged: {
            if (mouse.button == Qt.LeftButton) {
                lastX = mouse.x
                lastY = mouse.y
            }
        }

        onClicked: {
            if (mouse.button === Qt.LeftButton) {

                if(rulerPlugin){
                    if(rulerPlugin.used){
                        rulerPlugin.clickCoordinate=map.mouseClickCoordinate
                    }else{
                        control.clicked(mouseClickCoordinate);
                    }
                }else{
                    control.clicked(mouseClickCoordinate)
                }
            }else if (mouse.button === Qt.RightButton) {

                if(rulerPlugin){
                    if(rulerPlugin.used){
                        rulerPlugin.deleteLastItem()
                    }else{
                        control.menuRequested()

                        if(calibrationazdist){
                            calibrationazdist.calibration = false
                        }
                        if(sendpositionfix){
                            sendpositionfix.position_fix = false
                        }
                    }
                }else{
                    control.menuRequested()

                    if(calibrationazdist){
                        calibrationazdist.calibration = false
                    }
                    if(sendpositionfix){
                        sendpositionfix.position_fix = false
                    }
                }

            }
        }

        onDoubleClicked: {
            var mouseGeoPos = toCoordinate(Qt.point(mouse.x, mouse.y));
            var preZoomPoint = fromCoordinate(mouseGeoPos, false);
            if (mouse.button === Qt.LeftButton) {
                zoomLevel = Math.floor(zoomLevel + 1)
            } else if (mouse.button === Qt.RightButton) {
                zoomLevel = Math.floor(zoomLevel - 1)
            }
            var postZoomPoint = fromCoordinate(mouseGeoPos, false);
            var dx = postZoomPoint.x - preZoomPoint.x;
            var dy = postZoomPoint.y - preZoomPoint.y;

            center = toCoordinate(Qt.point(width / 2.0 + dx, height / 2.0 + dy));

            lastX = -1;
            lastY = -1;
        }

        onPressAndHold:{
            if (Math.abs(pressX - mouse.x ) < jitterThreshold
                    && Math.abs(pressY - mouse.y ) < jitterThreshold) {
                control.menuRequested()
            }
        }
    }
}

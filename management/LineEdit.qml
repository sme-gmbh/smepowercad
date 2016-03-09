import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

TextField {
    id: lineEdit
    height: 30
    style: TextFieldStyle {
        textColor: "#e4e4e4"
        placeholderTextColor: "#aeaeae"
        background: Rectangle {
            color: "#747474"
            border.color: (control.focus) ? "#3498db" : "#9b9b9b"
            border.width: 1
            radius: 2
        }
    }
}

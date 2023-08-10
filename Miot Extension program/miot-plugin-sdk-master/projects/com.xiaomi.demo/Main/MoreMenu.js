'use strict';

import { Device, DeviceEvent, Host } from "miot";
import React from 'react';
import { ActionSheetIOS, Image, ListView, PixelRatio, StyleSheet, Text, TouchableHighlight, View } from 'react-native';
import Logger from './Logger';
let BUTTONS = [
  '测试对话框',
  '确定'
];

export default class MoreMenu extends React.Component {

  constructor(props) {
    super(props);
    if (__DEV__ && console.warn) {
      console.warn('强烈推荐使用「通用设置项」: `miot/ui/CommonSetting`, 你可以在「首页」-「教程」-「插件通用设置项」中查看使用示例');
    }
    let ds = new ListView.DataSource({
      rowHasChanged: (r1, r2) => r1 !== r2
    });
    this._createMenuData();
    this.state = {
      dataSource: ds.cloneWithRows(this._menuData.map((o) => (o.name)))
    };
    Logger.trace(this);
  }

  _createMenuData() {
    this._menuData = [
      {
        'name': '你好，开发者！',
        'func': () => {
          this.onShowDidButtonPress();
        }
      },
      {
        'name': '弹出Alert',
        'func': () => {
          alert('测试对话框');
        }
      },
      {
        'name': '弹出ActionSheet',
        'func': () => {
          this.showActionSheet();
        }
      },
      {
        'name': 'REACT-ART',
        'func': () => {
          this.showReactART();
        }
      },
      {
        'name': '高德地图',
        'func': () => {
          this.props.navigation.navigate('mhMapDemo', { 'title': '高德地图Demo' });
        }
      },
      {
        'name': '新目录结构获取图片方式测试',
        'func': () => {
          this.props.navigation.navigate('imagePathDemo', { 'title': '新目录结构获取图片方式测试' });
        }
      },
      {
        'name': '修改设备名称',
        'func': () => {
          Host.ui.openChangeDeviceName();
        }
      },
      {
        'name': '设备共享',
        'func': () => {
          Host.ui.openShareDevicePage();
        }
      },
      {
        'name': '检查固件升级（新版固件升级页面）',
        'func': () => {
          Host.ui.openDeviceUpgradePage(0);
        }
      }, {
        'name': '检查固件升级（旧版固件升级页面）',
        'func': () => {
          Host.ui.openDeviceUpgradePage(1);
        }
      },
      {
        'name': '删除设备',
        'func': () => {
          Host.ui.openDeleteDevice();
        }
      },
      {
        'name': '删除设备时自定义提示',
        'func': () => {
          Host.ui.openDeleteDevice("😘 🍚 🐰");
        }
      },
      {
        'name': '安全设置',
        'func': () => {
          Host.ui.openSecuritySetting();
        }
      },
      {
        'name': '常见问题',
        'func': () => {
          Host.ui.openHelpPage();
        }
      },
      {
        'name': '反馈问题',
        'func': () => {
          Host.ui.openFeedbackInput();
        }
      },
      {
        'name': '语音设备授权',
        'func': () => {
          Host.ui.openVoiceCtrlDeviceAuthPage();
        }
      },
      {
        'name': '分享',
        'func': () => {
          Host.ui.openShareListBar('小米智能家庭', '小米智能家庭', { uri: 'https://avatars3.githubusercontent.com/u/13726966?s=40&v=4' }, 'https://iot.mi.com/new/index.html');
        }
      },
      {
        'name': '获取设备列表数据',
        'func': () => {
          Host.ui.getDevicesWithModel(Device.model).then((devices) => {
            alert(JSON.stringify(devices));
          }).catch((err) => {
            alert(`未获取到设备${ err.message }`);
          });
        }
      },
      {
        'name': "开启定时",
        'func': () => {
          Host.ui.openTimerSettingPageWithVariousTypeParams("power_on", ["on", "title"], 'off', "title");
        }
      },
      {
        'name': '位置管理',
        'func': () => {
          Host.ui.openRoomManagementPage();
        }
      },
      {
        'name': '时区设置',
        'func': () => {
          Host.ui.openDeviceTimeZoneSettingPage();
        }
      },
      {
        'name': '添加到桌面',
        'func': () => {
          Host.ui.openAddToDesktopPage();
        }
      },
      {
        'name': '蓝牙网关',
        'func': () => {
          Host.ui.openBtGatewayPage();
        }
      },
      {
        'name': 'Android手机蓝牙设置页面',
        'func': () => {
          Host.ui.openPhoneBluSettingPage();
        }
      }
    ];
  }

  componentDidMount() {
    this.listenter = DeviceEvent.deviceTimeZoneChanged.addListener((val) => {
      console.log("deviceTimeZoneChanged", val);
    });
  }

  componentWillUnmount() {
  }

  render() {
    return (
      <View style={styles.container}>
        <ListView style={styles.list} dataSource={this.state.dataSource} renderRow={this._renderRow.bind(this)} />
      </View>
    );
  }

  _renderRow(rowData, sectionID, rowID) {
    return (
      <TouchableHighlight underlayColor="#838383" onPress={() => {
        this._pressRow(rowID);
        Logger.trace(this, this._renderRow, { name: rowData });
      }}>
        <View>
          <View style={styles.rowContainer}>
            <Text style={styles.title}>{rowData}</Text>
            <Image style={styles.subArrow} source={require("../Resources/sub_arrow.png")} />
          </View>
          <View style={styles.separator}></View>
        </View>
      </TouchableHighlight>
    );
  }

  _pressRow(rowID) {
    console.log(`row${ rowID }clicked!`);
    this._menuData[rowID].func();
  }

  onShowDidButtonPress() {
    this.props.navigation.navigate('helloDeveloper');
  }

  showReactART() {
    this.props.navigation.navigate('helloReactART');
  }

  showActionSheet() {
    if (Host.isIOS)
      ActionSheetIOS.showActionSheetWithOptions({
        options: BUTTONS,
        destructiveButtonIndex: 1
      },
      (buttonIndex) => {
      });
  }
}

var styles = StyleSheet.create({
  container: {
    flex: 1,
    borderTopColor: '#f1f1f1',
    borderTopWidth: 1,
    flexDirection: 'row',
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#ffffff',
    marginBottom: 0,
    marginTop: 0
  },
  rowContainer: {
    height: 52,
    alignSelf: 'stretch',
    flexDirection: 'row',
    paddingLeft: 23,
    paddingRight: 23,
    alignItems: 'center',
    flex: 1
  },
  list: {
    alignSelf: 'stretch'
  },
  title: {
    fontSize: 15,
    color: '#333333',
    alignItems: 'center',
    flex: 1
  },
  subArrow: {
    width: 7,
    height: 14
  },
  separator: {
    height: 1 / PixelRatio.get(),
    backgroundColor: '#e5e5e5',
    marginLeft: 20
  }
});

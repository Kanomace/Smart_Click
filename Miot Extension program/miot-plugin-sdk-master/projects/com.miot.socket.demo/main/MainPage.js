import React from 'react';
import { Package, Host, Device, PackageEvent, Service, DeviceEvent } from 'miot';
import { View, Text, Image, StyleSheet, TouchableOpacity, ScrollView } from 'react-native';
import NavigationBar from 'miot/ui/NavigationBar';
import Separator from 'miot/ui/Separator';

/**
 * SDK 提供的多语言 和 插件提供的多语言
 */
import { strings as SdkStrings, Styles as SdkStyles } from 'miot/resources';
import PluginStrings from '../resources/strings';
/**
 * SDK 支持的字体
 */
import * as SdkFontStyle from 'miot/utils/fonts';
import { element } from "prop-types";

/**
 * wifi 类型设备通用模板， 提供了设备属性获取及订阅相关功能， 开发者在进行实现设备属性订阅相关功能时，需要修改相关参数
 */
export default class MainPage extends React.Component {

  /**
   * 页面内部自定义Header
   * @param navigation
   * @returns {{header: *}|{header: null}}
   */
  static navigationOptions = ({ navigation }) => {
    const { titleProps } = navigation.state.params || {};
    if (!titleProps) return { header: null };
    return {
      header: <NavigationBar { ...titleProps } />
    };
  };

  constructor(props) {
    super(props);
    this.initNavigationBar();
    this.state = {
      switch: false,
      countDown: ""
    };
  }

  initNavigationBar() {
    this.props.navigation.setParams({
      titleProps: {
        title: Device.name,
        left: [
          {
            key: NavigationBar.ICON.BACK,
            onPress: () => {
              Package.exit();
            }
          }
        ],
        right: [
          {
            key: NavigationBar.ICON.MORE,
            onPress: () => {
              // 跳转到设置页
              this.props.navigation.navigate('SettingPage', { title: SdkStrings.setting });
            }
          }
        ]
      }
    });
  }

  UNSAFE_componentWillMount() {
    this.addListener();
    this.getDevicePropsValue();
    this.refreshCountDown();
    this.loop = setInterval(this.refreshCountDown, 3000);
  }

  getDevicePropsValue() {

    /**
     * 这里的 siid 和 piid 的获取，可以通过解析 getSpecString 获取的spec信息， 也可以通过开发者平台上查询该设备对应的设备属性 siid 和 piid值
     */
    let params = [
      { did: Device.deviceID, siid: 2, piid: 1 }
    ];

    Service.spec.getPropertiesValue(params).then((res) => {
      console.log('getPropertiesValue success ', res);
      this.setState({
        switch: res[0].value
      });
    }).catch((error) => {
      console.log('getPropertiesValue error ', error);
    });
  }

  refreshCountDown = () => {
    Service.scene.loadTimerScenes(Device.deviceID).then(scenes => {
      console.log('loadTimerScenes', scenes);
      if (scenes && scenes.length > 0) {
        console.log('myTimerScenes', scenes[0].setting.off_time.split(' '));
        if (this.state.switch) {
          console.log('this.state.switch', this.state.switch);
          scenes.forEach(element => {
            console.log("element.setting.enable_timer_off", element.setting["enable_timer"]);
            if (element.identify == "switch_off" && element.setting["enable_timer"] == "1") {
              let date = this.getDate(element, true);
              console.log('date', `将在${ date }关闭`);
              this.setState({
                countDown: `将在${ date }关闭`
              });
            }
          });
        } else {
          console.log('this.state.switch', this.state.switch);
          scenes.forEach(element => {
            console.log('this.state.enable_timer_on', element.setting["enable_timer"]);
            if (element.identify == "switch_on" && element.setting["enable_timer"] == "1") {
              let date = this.getDate(element, false);
              console.log('date', `将在${ date }开启`);
              this.setState({
                countDown: `将在${ date }开启`
              });
            }
          });
        }
        let validScenes = scenes.filter(element => {
          return element.setting["enable_timer"] == "1";
        });
        console.log('validScenes', validScenes);
        if (validScenes.length == 0) {
          this.setState({
            countDown: ""
          });
        }
      }
    }).catch((e) => {
      console.log('e', e);
    });
  };

  getDate(element, isOn) {
    let time = isOn ? element.setting.off_time.split(' ') : element.setting.on_time.split(' ');
    return `${ time[3] < 10 ? '0'+ time[3] : time[3]}/${ time[2] < 10 ? '0'+ time[2] : time[2] } ${ time[1] < 10 ? '0'+ time[1] : time[1] }:${ time[0] < 10 ? '0'+ time[0] : time[0] }`;
  }

  addListener() {

    this.mPackageAuthorizationAgreed = PackageEvent.packageAuthorizationAgreed.addListener(() => {
      // 隐私弹窗-用户点击同意
      console.log('user agree protocol...');
    });

    /**
     * 对设备属性进行订阅
     * prop.属性名, profile 设备这样进行传参   eg: prop.power
     * prop.siid.piid， spec协议设备这样进行传参  eg: prop.2.1
     */
    Device.getDeviceWifi().subscribeMessages('prop.2.1').then((subcription) => {
      this.mSubcription = subcription;
    }).catch((error) => {
      console.log('subscribeMessages error', error);
    });

    // 监听设备属性发生变化事件； 当设备属性发生改变，会发送事件到js，此处会收到监听回调
    this.mDeviceReceivedMessages = DeviceEvent.deviceReceivedMessages.addListener(
      (device, map, data) => {
        console.log('Device.addListener', device, map, data);
        if (map.get("prop.2.1")) {
          console.log('switch', map.get("prop.2.1"));
          this.setState({
            switch: map.get("prop.2.1")[0]
          });
        }
      });
  }

  removeListener() {
    // 取消监听 隐私权限
    this.mPackageAuthorizationAgreed && this.mPackageAuthorizationAgreed.remove();
    // 取消订阅
    this.mSubcription && this.mSubcription.remove();
    // 取消监听
    this.mDeviceReceivedMessages && this.mDeviceReceivedMessages.remove();
    clearInterval(this.loop);
  }

  render() {

    return (
      <View style={ styles.container }>
        <Separator/>

        <View style={ { marginTop: 80, justifyContent: 'center', flexDirection: 'column', alignItems: 'center' } }>
          <Image
            style={ { width: 200, height: 200 } }
            source={ this.state.switch ? require('../resources/images/on.png') : require('../resources/images/off.png') }/>

          <Text style={ [styles.textSwitchStyle, { marginTop: 30 }] }>{ this.state.switch ? "已开启" : "已关闭" }</Text>
          <Text style={ [styles.textCountDownStyle, { marginTop: 10 }] }>{ this.state.countDown }</Text>
        </View>

        <TouchableOpacity style={ [styles.btnStyle, { marginTop: 150, flexDirection: "row" }] } onPress={ () => {
          this.changeSwitchStatus();
        } }>
          <Image
            style={ { width: 50, height: 50 } }
            source={ require('../resources/images/off.png') }/>
          <Text style={ [styles.textStyle, { marginLeft: 20 }] }>{ this.state.switch ? "关闭" : "开启" }</Text>
        </TouchableOpacity>

        <TouchableOpacity style={ [styles.btnStyle, { marginTop: 10, flexDirection: "row" }] } onPress={ () => {
          this.openCountDownPage();
        } }>
          <Image
            style={ { width: 50, height: 50, transform: [{ rotate: '45deg' }] } }
            source={ require('../resources/images/off.png') }/>
          <Text style={ [styles.textStyle, { marginLeft: 20 }] }>倒计时</Text>
        </TouchableOpacity>
      </View>
    );
  }

  componentWillUnmount() {
    this.removeListener();
  }


  changeSwitchStatus() {

    /**
     * 这里的 siid 和 piid 的获取，可以通过解析 getSpecString 获取的spec信息， 也可以通过开发者平台上查询该设备对应的设备属性 siid 和 piid值;
     * value 的数据类型 也可以通过 开发者平台上查询（和查看 siid 和 piid 相同）
     */
    let params = [
      { did: Device.deviceID, siid: 2, piid: 1, value: !this.state.switch }
    ];

    Service.spec.setPropertiesValue(params).then((res) => {
      console.log('setPropertiesValue success ', res);
      this.setState({
        switch: !this.state.switch,
        countDown: ``
      });
      this.refreshCountDown();
    }).catch((error) => {
      console.log('setPropertiesValue error ', error);
    });

  }

  openCountDownPage() {
    let params = {
      onMethod: "set_properties",
      offMethod: 'set_properties',
      onParam: [{
        did: Device.deviceID,
        siid: 2,
        piid: 1,
        value: true
      }],
      offParam: [{
        did: Device.deviceID,
        siid: 2,
        piid: 1,
        value: false
      }],
      identify: this.state.switch ? `switch_off` : `switch_on`,
      displayName: 'countdown'
    };
    Service.scene.openCountDownPage(this.state.switch, params);
  }
}


const styles = StyleSheet.create({
  container: {
    backgroundColor: SdkStyles.common.backgroundColor,
    flex: 1
  },
  textStyle: {
    fontSize: 16,
    lineHeight: 18,
    color: '#000'
  },
  textStyle1: {
    fontSize: 20,
    lineHeight: 22,
    color: '#333333',
    fontFamily: SdkFontStyle.FontKmedium,
    marginBottom: 20
  },
  btnStyle: {
    marginTop: 10,
    marginLeft: 10,
    marginRight: 10,
    padding: 20,
    borderRadius: 15,
    height: 80,
    backgroundColor: '#fff',
    justifyContent: 'flex-start',
    alignItems: 'center'
  },
  textSwitchStyle: {
    fontSize: 16,
    lineHeight: 18,
    color: '#000'
  },
  textCountDownStyle: {
    fontSize: 16,
    lineHeight: 18,
    color: '#aca7a7'
  }
});




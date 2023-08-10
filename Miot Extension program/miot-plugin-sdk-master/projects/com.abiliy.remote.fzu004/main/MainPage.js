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
      header: <NavigationBar {...titleProps} />
    };
  };

  constructor(props) {
    super(props);

    this.initNavigationBar();

    this.state = {
      switch: false
    };
  }



//顶部菜单栏
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
    Device.getDeviceWifi().subscribeMessages("prop.power", 'prop.2.4').then((subcription) => {
      this.mSubcription = subcription;
    }).catch((error) => {
      console.log('subscribeMessages error', error);
    });

    // 监听设备属性发生变化事件； 当设备属性发生改变，会发送事件到js，此处会收到监听回调
    this.mDeviceReceivedMessages = DeviceEvent.deviceReceivedMessages.addListener(
      (device, map, data) => {
        console.log('Device.addListener', device, map, data);
      });
  }

  removeListener() {
    // 取消监听 隐私权限
    this.mPackageAuthorizationAgreed && this.mPackageAuthorizationAgreed.remove();
    // 取消订阅
    this.mSubcription && this.mSubcription.remove();
    // 取消监听
    this.mDeviceReceivedMessages && this.mDeviceReceivedMessages.remove();
  }

  render() {

    return (
      <View style={styles.container}>
        <Separator/>
        <ScrollView style={{ flex: 1 }}>
          <View style={{ marginTop: 20, justifyContent: 'center', flexDirection: 'column', alignItems: 'center' }}>
            <Text style={styles.textStyle1}>你好，欢迎使用Smart Click</Text>
            <Image
/*               style={{ width: 350, height: 200 }}
              source={require('../resources/images/welcome.png')} /> */
              style={ { width: 300, height: 300 } }
              source={ this.state.switch ? require('../resources/images/device_on.png') : require('../resources/images/device_off.png') }/>
          </View>
          <View style={{ padding: 20 }}>
            <TouchableOpacity style={[styles.btnStyle , {backgroundColor : "#00000000",height:50}]} onPress={() => { this.setDevicePropsValue(); }}>
              <Image
                style={ { width: 150, height: 150 } }
                source={ this.state.switch ? require('../resources/images/switch_off.png') : require('../resources/images/switch_on.png') }/>
            </TouchableOpacity>
            <View style={{ padding: 20 }}></View>

            {/* <Text style={{ color: '#d71345', fontSize: 14 } }>.支持spec协议的设备(新设备)-设备属性订阅相关接口使用!</Text> */}
            <TouchableOpacity style={styles.btnStyle} onPress={() => { this.getDeviceSpecInfo(); }}>
              <Text style={styles.textStyle}>获取设备Spec信息</Text>
            </TouchableOpacity>
            <TouchableOpacity style={styles.btnStyle} onPress={() => { this.getDevicePropsValue(); }}>
              <Text style={styles.textStyle}>获取设备属性值</Text>
            </TouchableOpacity>
            <TouchableOpacity style={styles.btnStyle} onPress={() => { this.setDevicePropsValue(); }}>
              <Text style={styles.textStyle}>设置设备属性值</Text>
            </TouchableOpacity>
{/*             <TouchableOpacity style={styles.btnStyle} onPress={() => { this.doDeviceAction(); }}>
              <Text style={styles.textStyle}>请求调用设备的方法</Text>
            </TouchableOpacity> */}

          </View>
        </ScrollView>
      </View>
    );
  }

  componentWillUnmount() {
    this.removeListener();
  }

  getDeviceSpecInfo() {
    Service.spec.getSpecString(Device.deviceID).then((specInfo) => {
      console.log('spec info: ', specInfo);
    }).catch((error) => {
      console.log('getSpecString error', error);
    });
  }

  getDevicePropsValue() {

    /**
     * 这里的 siid 和 piid 的获取，可以通过解析 getSpecString 获取的spec信息， 也可以通过开发者平台上查询该设备对应的设备属性 siid 和 piid值
     */
    let params = [
      { did: Device.deviceID, siid: 2, piid: 4 },
      //{ did: Device.deviceID, siid: 2, piid: 1 }
    ];

    Service.spec.getPropertiesValue(params).then((res) => {
      console.log('getPropertiesValue success ', res);
    }).catch((error) => {
      console.log('getPropertiesValue error ', error);
    });
  }

  setDevicePropsValue() {

    /**
     * 这里的 siid 和 piid 的获取，可以通过解析 getSpecString 获取的spec信息， 也可以通过开发者平台上查询该设备对应的设备属性 siid 和 piid值;
     * value 的数据类型 也可以通过 开发者平台上查询（和查看 siid 和 piid 相同）
     */
    let params = [
      { did: Device.deviceID, siid: 2, piid: 4, value: !this.state.switch },
      //{ did: Device.deviceID, siid: 2, piid: 1, value: false }
    ];

    Service.spec.setPropertiesValue(params).then((res) => {
      console.log('setPropertiesValue success ', res);
      this.setState({
        switch:!this.state.switch
      })
    }).catch((error) => {
      console.log('setPropertiesValue error ', error);
    });
  }

/*   doDeviceAction() {

    let params = { did: Device.deviceID, siid: 1, aiid: 3, in: [17, "shanghai"] };
    Service.spec.doAction(params).then((res) => {
      console.log('doAction success ', res);
    }).catch((error) => {
      console.log('doAction error ', error);
    });
  } */

}



const styles = StyleSheet.create({
  container: {
    backgroundColor: SdkStyles.common.backgroundColor,
    flex: 2
  },
  textStyle: {
    fontSize: 16,
    lineHeight: 18,
    color: '#ffffff'
  },
  textStyle1: {
    fontSize: 20,
    lineHeight: 22,
    color: '#333333',
    fontFamily: SdkFontStyle.FontKmedium,
    marginBottom: 20
  },
  btnStyle: {
    marginTop: 20,
    marginLeft: 10,
    marginRight: 10,
    height: 40,
    backgroundColor: '#2196F3',
    justifyContent: 'center',
    alignItems: 'center'
  }
});




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
import { SlideGear } from "miot/ui/Gear";
import Ratio from './Ratio';
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
      switch: false,
      brightness:1,
      color_temperature:3000
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
    Device.getDeviceWifi().subscribeMessages('prop.2.1','prop.2.2','prop.2.3').then((subcription) => {
      this.mSubcription = subcription;
    }).catch((error) => {
      console.log('subscribeMessages error', error);
    });

    // 监听设备属性发生变化事件； 当设备属性发生改变，会发送事件到js，此处会收到监听回调
    this.mDeviceReceivedMessages = DeviceEvent.deviceReceivedMessages.addListener(
      (device, map, data) => {
        console.log('Device.addListener', device, map, data);
        if (map.get("prop.2.1")) {
          this.setState({
            switch:map.get("prop.2.1")[0]
          });
          console.log('上报开关',map.get("prop.2.1")[0] );
        }
        if (map.get("prop.2.2")) {
          this.setState({
            brightness:map.get("prop.2.2")[0]
          });
          console.log('上报亮度',map.get("prop.2.2")[0] );
        }
        if (map.get("prop.2.3")) {
          this.setState({
            color_temperature:map.get("prop.2.3")[0]
          });
          console.log('上报色温',map.get("prop.2.3")[0] );
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
  }

  render() {

    return (
      <View style={styles.container}>
        <Separator/>
        <ScrollView style={{ flex: 1 }}>

          <View style={ { marginTop: 80, justifyContent: 'center', flexDirection: 'column', alignItems: 'center' } }>
            <Image
              style={ { width: 300, height: 300 } }
              source={ this.state.switch ? require('../resources/images/device_on.png') : require('../resources/images/device_off.png') }/>

          </View>
          <View style={{ padding: 20 }}>
            <TouchableOpacity style={[styles.btnStyle , {backgroundColor : "#00000000",height:50}]} onPress={() => { this.switchLight(); }}>
              <Image
                style={ { width: 150, height: 150 } }
                source={ this.state.switch ? require('../resources/images/switch_off.png') : require('../resources/images/switch_on.png') }/>
            </TouchableOpacity>
            <Text style={{ color: '#fff', fontSize: 14,marginTop: 50 } }>调节亮度</Text>
            <SlideGear
              optionMin={1}
              optionMax={100}
              optionStep={1}
              value={this.state.brightness}
              containerStyle={styles.sliderStyle}
              blockStyle={{ backgroundColor: '#f48e1f' }}
              minimumTrackTintColor="white"
              leftTextColor="#f48e1f"
              rightTextColor="#f48e1f"
              contentType={SlideGear.CONTENTTYPE.NUM}
              onValueChange={(index) => {
                 this.setState({ brightness: index });
              }}
              onSlidingComplete={(index) => {
                this.updateBrightness();
                console.log('onSlidingComplete: ', index);
              }}
              showEndText={false}
            />
            <Text style={{ color: '#81d8cf', fontSize: 14 ,marginTop: 30} }>调节色温</Text>
            <SlideGear
              optionMin={3000}
              optionMax={6400}
              optionStep={1}
              value={this.state.color_temperature}
              containerStyle={styles.sliderStyle}
              blockStyle={{ backgroundColor: '#f48e1f' }}
              minimumTrackTintColor="white"
              leftTextColor="#f48e1f"
              rightTextColor="#f48e1f"
              contentType={SlideGear.CONTENTTYPE.NUM}
              onValueChange={(index) => {
                this.setState({ color_temperature: index });
              }}
              onSlidingComplete={(index) => {
                this.updateColorTemperature();
                console.log('onSlidingComplete: ', index);
              }}
              showEndText={false}
            />
          </View>
        </ScrollView>
      </View>
    );
  }

  componentWillUnmount() {
    this.removeListener();
  }


  switchLight(){
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
        switch:!this.state.switch
      })
    }).catch((error) => {
      console.log('setPropertiesValue error ', error);
    });
  }

  updateBrightness(){
    /**
     * 这里的 siid 和 piid 的获取，可以通过解析 getSpecString 获取的spec信息， 也可以通过开发者平台上查询该设备对应的设备属性 siid 和 piid值;
     * value 的数据类型 也可以通过 开发者平台上查询（和查看 siid 和 piid 相同）
     */
    let params = [
      { did: Device.deviceID, siid: 2, piid: 2, value: this.state.brightness }
    ];

    Service.spec.setPropertiesValue(params).then((res) => {
      console.log('setPropertiesValue success ', res);
    }).catch((error) => {
      console.log('setPropertiesValue error ', error);
    });
  }

  updateColorTemperature(){
    /**
     * 这里的 siid 和 piid 的获取，可以通过解析 getSpecString 获取的spec信息， 也可以通过开发者平台上查询该设备对应的设备属性 siid 和 piid值;
     * value 的数据类型 也可以通过 开发者平台上查询（和查看 siid 和 piid 相同）
     */
    let params = [
      { did: Device.deviceID, siid: 2, piid: 3, value: this.state.color_temperature }
    ];

    Service.spec.setPropertiesValue(params).then((res) => {
      console.log('setPropertiesValue success ', res);
    }).catch((error) => {
      console.log('setPropertiesValue error ', error);
    });
  }


  getDevicePropsValue() {

    /**
     * 这里的 siid 和 piid 的获取，可以通过解析 getSpecString 获取的spec信息， 也可以通过开发者平台上查询该设备对应的设备属性 siid 和 piid值
     */
    let params = [
      { did: Device.deviceID, siid: 2, piid: 1 },
      { did: Device.deviceID, siid: 2, piid: 2 },
      { did: Device.deviceID, siid: 2, piid: 3 },
    ];

    Service.spec.getPropertiesValue(params).then((res) => {
      console.log('getPropertiesValue success ', res);
      this.setState({
        switch: res[0].value || false,
        brightness: res[1].value || 1,
        color_temperature: res[2].value || 3000,
      })
    }).catch((error) => {
      console.log('getPropertiesValue error ', error);
    });
  }


}

const styles = StyleSheet.create({
  container: {
    backgroundColor: "#558bac",
    flex: 1
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
    marginTop: 10,
    marginLeft: 10,
    marginRight: 10,
    height: 40,
    backgroundColor: '#2196F3',
    justifyContent: 'center',
    alignItems: 'center'
  },
  sliderStyle: {
    width: Ratio.width * 0.85,
    height: Ratio.convertY(25),
    color: 'white',
    alignSelf: 'center',
    top: Ratio.convertY(15),
    bottom: Ratio.convertY(2)
  },
});




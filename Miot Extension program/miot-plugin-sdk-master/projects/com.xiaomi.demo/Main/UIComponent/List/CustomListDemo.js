'use strict';

import { ListItem, ListItemWithSlider, ListItemWithSwitch } from 'miot/ui/ListItem';
import Separator from 'miot/ui/Separator';
import React from 'react';
import { Dimensions, ScrollView, View } from 'react-native';
import Logger from '../../Logger';

const { width } = Dimensions.get('window');

export default class CustomListDemo extends React.Component {

  constructor(props, context) {
    super(props, context);
    this.props.navigation.setParams({
      title: "列表项"
    });
    Logger.trace(this);
  }

  state = {
    switchOn: false
  };

  render() {
    return (
      <View style={{ backgroundColor: '#f2f2f2', flex: 1 }}>
        <Separator />
        <ScrollView>
          <View style={{ alignItems: 'center' }}>
            <ListItem
              title="只有标题"
              showDot={true}
              onPress={() => console.log(0)}
              accessible={true}
              accessibilityHint="press title"
              leftIcon={require('../images/love-active.jpg')}
            />
            <ListItem
              title="标题加右侧文案"
              value="小米米家智能墙壁开关(单火线单键版)"
              showDot={true}
              onPress={() => console.log(1)}
              accessible={false}
            />
            <ListItem
              title="标题加副标题"
              subtitle="这是用来测试副标题的文案，尽量写长一点争。"
              showDot={true}
              onPress={() => console.log(2)}
            />
            <ListItem
              title="标题加副标题加右侧文案标题加副标题加右侧文案"
              subtitle="这是用来测试副标题的文案，尽量写长一点争取可用来测试副标题的文案，尽量写长一点争取可用来测试副标题的文案，尽量写长一点争取可以换行。"
              showDot={true}
              value="测试右侧文案，写长一点争取可以截断。"
              onPress={() => console.log(3)}
              accessible={true}
              accessibilityHint="press title again"
              unlimitedHeightEnable
              subtitleNumberOfLines={4}
            />
            <ListItem
              title="标题加副标题加右侧文案标题加副标题加右侧文案"
              subtitle="这是用来测试副标题的文案，尽量写长一点争取可以换行。"
              showDot={true}
              value="测试右侧文案，写长一点争取可以截断。"
              onPress={() => console.log(3)}
              accessible={true}
              accessibilityHint="press title again"
              leftIcon={require('../images/love-active.jpg')}
            />
            <ListItem
              title="not show separator"
              showSeparator={false}
            />
            <ListItem
              title="hideArrow"
              hideArrow={true}
            />
            <ListItem
              title="自定义样式-ABCabc123测试ABCabc123测试ABCabc123测试ABCabc123测试"
              subtitle="这是用来测试副标题的文案，尽量写长一点争取可以换行。"
              showDot={true}
              value="这是一段测试右侧文案"
              containerStyle={{ width: width * 0.8, backgroundColor: 'lightblue' }}
              titleStyle={{ fontSize: 17, color: 'red' }}
              subtitleStyle={{ fontSize: 10, color: 'green' }}
              valueStyle={{ fontSize: 10, color: 'yellow' }}
              onPress={() => console.log(4)}
              separator={<Separator />}
            />
            <ListItemWithSlider
              title="被弃用的滑动条列表项"
              disabled={true}
              onSlidingComplete={(value) => console.log(value)}
            />
            <ListItemWithSlider
              title="bc123列用的滑动条测试ABCab项"
              useNewType
              onSlidingComplete={(value) => console.log(value)}
            />
            {/* <ListItemWithSlider
              title="ABCabc123测试ABCabc123测试ABCabc123测试ABCabc123测试"
              sliderProps={{ minimumValue: 25, maximumValue: 75, value: 60 }}
              sliderStyle={{
                minimumTrackTintColor: "red",
                maximumTrackTintColor: "#fff",
                style: { width: width * 0.5, alignSelf: 'center' },
                trackStyle: { height: 4, borderRadius: 2 },
                thumbStyle: { width: 30, height: 30, borderRadius: 15 }
              }}
              containerStyle={{ width: width * 0.8, backgroundColor: 'lightblue' }}
              titleStyle={{ fontSize: 17, color: 'red' }}
              valueStyle={{ fontSize: 10, color: 'yellow' }}
              showWithPercent={false}
              onSlidingComplete={(value) => console.log('onSlidingComplete: ', value)}
              onValueChange={(value) => console.log('onValueChange: ', value)}
              separator={<Separator />}
              accessibilityLabel="slider"
            /> */}
            <ListItemWithSwitch
              title="我🍋了"
              value={this.state.switchOn}
              onValueChange={(value) => {
                this.setState({
                  switchOn: value
                });
              }}
              leftIcon={require('../images/love-active.jpg')}
            />
            <ListItemWithSwitch
              title="一直自闭"
              value={true}
              disabled={true}
              subtitle="开启自闭模式"
              onValueChange={(value) => console.log(value)}
            />
            <ListItemWithSwitch
              type={'button'}
              title="一直带按钮"
              valueText="now-my last day"
              subtitle="开启自眠模式休眠模式休眠模式休眠模式休眠模式休眠模式休眠模式"
              unlimitedHeightEnable
              buttonOption={{
                onPress: () => alert('asd')
              }}
            />
            <ListItemWithSwitch
              type={'choice'}
              title="一直带单选"
              valueText="now-my last day"
              choiceOption={{
                checked: true,
                checkedColor: 'red',
                onValueChange: () => alert('asd')
              }}
            />
            <ListItemWithSwitch
              type={'sort'}
              title="一直带排序"
              valueText="now-my last day"
              subtitle="开启自闭模眠模式休眠模式休眠模式休眠模式休眠模式休眠模式休式"
              sortOption={{
                onPress: () => alert('点按'),
                onLongPress: () => alert('长按')

              }}
            />
            <ListItemWithSwitch
              title="无法开启的自嗨模式"
              disabled={true}
              value={false}
              valueText="23:00-次日4:00"
              onPress={() => console.log('do what u want to do')}
              onValueChange={(value) => console.log(value)}
            />
            <ListItemWithSwitch
              title="休眠模式休眠模式休眠模式休眠模式休眠模式休眠模式休眠模式休眠模式"
              valueText="now-my last day"
              subtitle="开启后将长眠不醒开启后将长眠不醒开启后将长眠不醒"
              onPress={() => console.log('do what u want to do')}
              onValueChange={(value) => console.log(value)}
              leftIcon={require('../images/love-active.jpg')}
            />
            <ListItemWithSwitch
              title="ABCabc123测试ABCabc123测试ABCabc123测试ABCabc123测试"
              valueText="测试测试测试测试测试测试测试测试测试测试测试"
              subtitle="副标题测试副标题测试副标题测试副标题测试副标题测试副标题测试"
              onPress={() => console.log('do what u want to do')}
              onValueChange={(value) => console.log(value)}
              containerStyle={{ width: width * 0.8, height: 90, backgroundColor: 'lightblue' }}
              titleStyle={{ fontSize: 17, color: 'red' }}
              subtitleStyle={{ fontSize: 10, color: 'green' }}
              valueTextStyle={{ fontSize: 10, color: 'yellow' }}
              separator={<Separator />}
            />
          </View>
        </ScrollView>
      </View>
    );
  }
}

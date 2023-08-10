---
name: 📝Issue 模版
about: Github issue机制已被废弃，请去小米开发者平台的工单系统提工单，新创建的这个 issue 将被关闭。
title: ''
labels: ''
assignees: zanetti4

---
**注意事项：（阅读后，创建issue时此段可删除）**
- Issue提交及后续处理流程中，均需指定assignee，自己处理完毕后需要其他人继续跟进的，请及时更改assignee（创建issue时assignee默认是米家这边）
- 每个issue需要指定待修复的SDK版本，通过milestone指定（米家指定）
- 如果有报错，一定要贴出报错的错误信息，否则米家无法排查
- 请issue处理人指定issue的优先级（米家+开发者协商）
    紧急优先级：线上插件的严重bug 或 block验收的新产品接入问题。12小时内响应，48小时内解决
    高优先级：排期紧张(<1月)的新产品接入相关问题。24小时内响应，1周内解决
    普通优先级：已有老框架插件功能补齐及迁移；排期较宽松(>1月)的新产品接入；已有模块的性能优化等。24小时内响应，3周内解决

**请自查：（不用回答，创建issue时此段可删除）**
- IPA/APK 是否更新到最新？
- SDK 是否更新到最新？
- Check 文档了吗？
- 是否尝试搜索issue？
- 是否尝试搜索“常见问题”

---
**是否为新品（必填）**
填 新品 or 在售

**关联的产品model**
此issue关联的产品model，如“xiaomi.demo.v1”

**项目ID（必填）**
项目ID是指小米生态链部立项的项目ID，未在生态链部立项的项目填【无项目ID】

**用户ID（必填）**
发现问题的账号id，**强烈建议**在提交issue之前，现在米家APP的“帮主和反馈”中提交问题的说明，并**勾选上传问题日志**，那么我们后台就会根据
当前的用户ID，来查询出现问题的日志，方便大家更快速的解决问题。

**企业名称（必填）**
贵司的名称

**环境（必填）**
- 电脑操作系统:[eg.Windows/Mac OS/Linux]
- 手机系统:[eg.Android 8.1/iOS 11.0]
- SDK分支: [eg.master/10000/...]
- 相关模块:[eg.Host/Service/Account]

**现象（必填）**
简单明了地描述一下实际的现象。

**期望（必填）**
简单明了地描述一下期望的表现。

**其他的上下文/日志/截图**
提供更多的上下文，有日志或者截图更好。

**尝试过的解决办法**
尝试过哪些操作？

**不要贴大段代码❗️**
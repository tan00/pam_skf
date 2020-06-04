本项目使用支持skf接口的usbkey，编写PAM 服务提供者接口 (service provider interface, SPI)
应用于需要使用usbkey对用户身份进行验证的场景

1. 发key过程
用户第一次登录时，如果发现key内没有认证数据， 则将用户名和密码的摘要
通过SKF_WriteFile写入key中，并写入已初始化标记， 完成发key动作。

2. 认证过程， SKF_ReadFile ， 对比用户名和密码， 如果一致，则验证通过。



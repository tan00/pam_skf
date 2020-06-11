# 原理
通过linux pam接口进行登录认证，调用gm0016 的skf函数接口 编写认证动态库，由需要进行认证的程序调用

# 编译方法
将厂商的skf接口编译为libes_3000gm.a，放到项目目录，使用cmake常规方式编译

# 使用方法
1. 编译或解压压缩包，得到 ukey_tool工具 和 libpam_skf.so动态库 ， 将 libpam_skf.so  拷贝到 /lib64/security目录下
2. 插入ukey，使用  ukey_tool 程序，对该key进行格式化， 如果该key为空白key，则调用 ukey_tool init , 如果该key已有其他app，则依次调用 ukey_tool reset 、 ukey_tool init
3. 修改  /etc/pam.d 下需要使用ukey进行认证的程序配置，例如本地登录 login 程序
    修改/etc/pma.d/login 文件增加如下红色内容：

```    
#%PAM-1.0
auth [user_unknown=ignore success=ok ignore=ignore default=bad] pam_securetty.so
auth       substack     system-auth
auth       include      postlogin      
auth       requisite     libpam_skf.so  #增加这一行
account    required     pam_nologin.so
```

4. 如需ssh登录验证 在 `/etc/pma.d/sshd` 中加入 ， 如需图形界面登录验证，在 `/etc/pma.d/gdm-password` 中加入（适用于gnome桌面）


# 内部流程 
1. 发key过程
        用户第一次登录时，如果发现key内没有认证数据， 则将用户名和密码的摘要写入key中，并写入已初始化标记， 完成发key动作
2. 认证过程 
        对比用户名和密码， 如果一致，则验证通过。
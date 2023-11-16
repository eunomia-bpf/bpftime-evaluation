import frida

# 连接到目标进程
session = frida.attach('target_process')

# 定义我们的插桩函数
def on_enter(args):
    print('Function entered!')

def on_leave(retval):
    print('Function exited!')

# 创建一个拦截器
interceptor = session.create_script("""
Interceptor.attach(ptr('0x12345678'), {
    onEnter: function(args) {
        send('Function entered!');
    },
    onLeave: function(retval) {
        send('Function exited!');
    }
});
""")

# 定义消息处理函数
def on_message(message, data):
    print(message['payload'])

interceptor.on('message', on_message)

# 加载并运行拦截器
interceptor.load()

# 进入主循环，等待事件发生
sys.stdin.read()

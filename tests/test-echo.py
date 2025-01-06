from pwn import log
from utils import setup, start

setup()
p = start()

msg = b'Hello, World!\n'
p.send(msg)
assert p.recvall(timeout=.5) == msg
assert p.poll() == 0

log.success('Test successed')

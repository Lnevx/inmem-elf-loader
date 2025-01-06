from pwn import *
from config import *

context.log_level = 'error'

if not args.LOADER or not args.TARGET:
    raise Exception

loader = context.binary = ELF(args.LOADER)
with open(args.TARGET, 'rb') as file:
    payload_elf = file.read()

shellcode = loader.get_section_by_name('.text').data() + payload_elf

p = run_shellcode(shellcode, vma=SC_BASE_ADDR)

msg = b'Hello, World!\n'
p.send(msg)
assert p.recvall(timeout=RECV_TIMEOUT) == msg
assert p.poll() == 0

log.success('Test successed')

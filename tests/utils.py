import pwn
from pwn import ELF, args, context

SC_BASE_ADDR = 0x1337000


def setup():
    context.log_level = 'error'


def start():
    if not args.LOADER or not args.TARGET:
        raise Exception

    loader = context.binary = ELF(args.LOADER)
    with open(args.TARGET, 'rb') as file:
        payload_elf = file.read()

    shellcode = loader.get_section_by_name('.text').data() + payload_elf

    return pwn.run_shellcode(shellcode, vma=SC_BASE_ADDR)

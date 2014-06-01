#
# boot.s -- Место, откуда начинается ядро. Также определяет заголовок мультизагрузки multiboot.
# За основу взят файл start.asm из руководства по разработке ядра Bran's kernel development tutorial
#

.set MBOOT_PAGE_ALIGN, 1<<0    # Загружает ядро и модули на границу страницы
.set MBOOT_MEM_INFO, 1<<1    # Предоставляет вашему ядру информацию о памяти
.set MBOOT_HEADER_MAGIC, 0x1BADB002 # Магическое значение Multiboot Magic
# ЗАМЕЧАНИЕ: Мы не используем значение MBOOT_AOUT_KLUDGE. Это значит, что GRUB 
# не передает нам символьную таблицу.
.set MBOOT_HEADER_FLAGS, MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
.set MBOOT_CHECKSUM, -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


.code32                        # Все инструкции должны быть 32-битовыми.

.section header
.global mboot                   # Собираем 'mboot' из кода на C.
.extern code                    # Начало секции '.text'.
.extern bss                     # Начало секции .bss .
.extern end                     # Конец последней загружаемой секции.

mboot:
  .long  MBOOT_HEADER_MAGIC     # GRUB будет искать это значение для каждой 
                                # 4-байтной границы в файле вашего ядра
  .long  MBOOT_HEADER_FLAGS     # Указывает, как GRUB должен загружать   ваш файл / настройки
  .long  MBOOT_CHECKSUM         # Чтобы обеспечить, чтобы приведенные выше значения были корректными

  .long  mboot                  # Место размещения дескриптора
  .long  code                   # Начало секции '.text' (код) ядра.
  .long  bss                    # Начало секции '.data' ядра.
  .long  end                    # Конец ядра.
  .long  start                  # Точка входа в ядро (первоначальный EIP).

.section bootstrap
.global start                   # Точка входа в ядро.
.extern main                    # Это точка входа в ваш код на C

start:
  push %ebx                   # Загрузка указателя на место размещения заголовка multiboot

  # Выполнение ядра:
  cli                         # Отключаем прерывания.
  call clearscreen
  movw $0x0F34, 0x000B8002

  call main                   # вызываем нашу функцию main().

die:
  hlt
  jmp die

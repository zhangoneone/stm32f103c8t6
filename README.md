# stm32f103c8t6

基于stm32f103c8t6开发，兼容stm32f10x系列。

stm32的基础架构服务，不涉及业务逻辑，作为stm32的工程模板和硬件驱动。

os kernel:freertos     
          
memorymanage: heap_4             =============>使用了freertos提供的内存管理方案4

fs:fatfs                         =============>增加虚拟扇区支持，1物理扇区=2虚拟扇区

io:xprintf                       =============>改进为线程安全io

shell:nr_micro_shell             =============>实现了部分linux命令

tcp/ip:lwip						 =============>未在此分支实现，在master分支上实现


在未作裁剪的情况下，使用以上模块的全部功能，大约占用stm32f103c8t6一半左右的ram和rom资源。


此分支暂时封存，转向master分支，延续项目的后续开发。


master分支采用stm32f103zet6



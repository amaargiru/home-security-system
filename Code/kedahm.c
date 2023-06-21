// Список изменений - в файле "Изменения.txt" в корневой папке проекта
// Табуляция в тексте программы = 3 символа
// Программа работы блока GSM-Alarm
// Процессор ATmega128
// Тактовая частота процессора - 8.000 МГц

#define MasterClock 8000000

#include "kedahm.h"

main()
{
    BeginInit();  // Начальная инициализация периферии микроконтроллера
    ClearRxBuffer();  // Очищаем приемный буфер UART1
    asm("SEI");  // Разрешаем прерывания
    SetActiveUart(UART_0);
    printf(">>\n\r");
    SetActiveUart(UART_1);  // Основной порт - для работы с GSM-модулем
    SIMNum = 0;  // Выбираем SIM-карту, с которой начнется опрос

    while (1)  // Основной цикл программы
    {
        SystemUpdate();  // Обновление системных переменных
        iButtonUpdate();  // Определяет режим работы с ключами iButton - запись, стирание, нормальный режим
        LoopsRead();  // Определение состояния охранных шлейфов
        LoopTest();  // Анализируем состояние охранных шлейфов

        while (iButtonMode != 0)  // Включен режим записи iButton-ключей
        {
            iButtonUpdate();  // Обновляем переменную iButtonMode
            if (iButtonMode == 1)
                iButtonNewKeysSearch();  // Запись новых ключей
            if (iButtonMode == 2)
                iButtonKeysErase();  // Удаление ключей
        }

        if ((!AbRecReadComplete) && (GSMStatus != 0))  // Если записи абонентов до сих пор не прочитаны, а GR-47 уже включен, предпринимаем очередную попытку
        {
            AbRecReadComplete = ReadAbRecords();  // Переопределяем состояние адресной книги
            SIMFault = AbRecReadComplete;  // Переопределяем состояние SIM-карты
        }

        if ((LoopViolation == 1) && (GuardSource != 0) && (TmGuardOffTimeout == 0))
        {
            GuardReaxion();  // Произошло срабатывание охранной системы
            LoopViolation = 0;  // Очищаем переменную, говорящую о срабатывании охранной системы
            PreparationGuardTime = 0;  // Через 0 минут (fw115, пред. версия - fw083, в которой было 3 минуты) осуществляем повторный переход в режим охраны
        }
    }
}

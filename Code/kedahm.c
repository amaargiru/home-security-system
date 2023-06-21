// ������ ��������� - � ����� "���������.txt" � �������� ����� �������
// ��������� � ������ ��������� = 3 �������
// ��������� ������ ����� GSM-Alarm
// ��������� ATmega128
// �������� ������� ���������� - 8.000 ���

#define MasterClock 8000000

#include "kedahm.h"

main()
{
    BeginInit();  // ��������� ������������� ��������� ����������������
    ClearRxBuffer();  // ������� �������� ����� UART1
    asm("SEI");  // ��������� ����������
    SetActiveUart(UART_0);
    printf(">>\n\r");
    SetActiveUart(UART_1);  // �������� ���� - ��� ������ � GSM-�������
    SIMNum = 0;  // �������� SIM-�����, � ������� �������� �����

    while (1)  // �������� ���� ���������
    {
        SystemUpdate();  // ���������� ��������� ����������
        iButtonUpdate();  // ���������� ����� ������ � ������� iButton - ������, ��������, ���������� �����
        LoopsRead();  // ����������� ��������� �������� �������
        LoopTest();  // ����������� ��������� �������� �������

        while (iButtonMode != 0)  // ������� ����� ������ iButton-������
        {
            iButtonUpdate();  // ��������� ���������� iButtonMode
            if (iButtonMode == 1)
                iButtonNewKeysSearch();  // ������ ����� ������
            if (iButtonMode == 2)
                iButtonKeysErase();  // �������� ������
        }

        if ((!AbRecReadComplete) && (GSMStatus != 0))  // ���� ������ ��������� �� ��� ��� �� ���������, � GR-47 ��� �������, ������������� ��������� �������
        {
            AbRecReadComplete = ReadAbRecords();  // �������������� ��������� �������� �����
            SIMFault = AbRecReadComplete;  // �������������� ��������� SIM-�����
        }

        if ((LoopViolation == 1) && (GuardSource != 0) && (TmGuardOffTimeout == 0))
        {
            GuardReaxion();  // ��������� ������������ �������� �������
            LoopViolation = 0;  // ������� ����������, ��������� � ������������ �������� �������
            PreparationGuardTime = 0;  // ����� 0 ����� (fw115, ����. ������ - fw083, � ������� ���� 3 ������) ������������ ��������� ������� � ����� ������
        }
    }
}

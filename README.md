# math  
## Постановка задачи  
Найти численное решение линейного уравнения переноса u_t+a*u_x=0, при a=1 с помощью схемы Годунова(разность назад) и неявной разностной схемы на отрезке [0,10]. Начальные данные: u(x,0)=5, если x<=3, u(x,0)=1, если x>3. Шаг по пространству можно взять равным 1/10. Провести расчеты при r=a*tau/h={0.25, 0.5, 1, 1.25} на два момента времени T={1; 5}. Для каждого r построить графики решения на моменты времени T={0; 1; 5}(т.е. 4 рисунка для каждой схемы, на каждом рисунке будут 3 графика на моменты времени T={0; 1; 5}). Для каждого момента времени отрисовать точное решение, графики точного решения отрисовать пунктиром. Полученные результаты прокомментировать, оформить в виде небольшого отчета.  

Для решения по неявной схеме понадобится метод прогонки.

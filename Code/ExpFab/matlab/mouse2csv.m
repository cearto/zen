% Mouse2CSV
clear all
load 'mouse/CESAR_1MOUSE (1).mat'
Mouse1 = Mouse1 ./ norm(Mouse1);
Mouse1 = Mouse1 + min(min(Mouse1));

csvwrite('mouse.csv', Mouse1);
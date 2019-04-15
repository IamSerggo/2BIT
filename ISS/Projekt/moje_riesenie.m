%uloha cislo 1 -------------------
%otvorim subor a vyberiem vsetky potrebne info
[sn, Fs] = audioread("xbabic09.wav");
samples = length(sn);

%uloha cislo 2 -------------------
%zoberiem 8. vzorku zo segmentu 16tich vzoriek
%pokial je vzroka > 0 - binarny symbol je 1, inak je 0
eigthbit_sn = sn(8:16:end);
for i = 1:2000
  if eigthbit_sn(i) > 0
    sn_binary_symbols(i) = 1;
  else
    sn_binary_symbols(i) = 0;
  endif
endfor

figure(1)
plot(sn(1:Fs/200))
hold on
stem((1:20)*4, sn_binary_symbols(1:20))
hold off

%uloha cislo 3 -------------------
%pre vypocet nulovych bodov a polov pouzijem funkciu zplane()
B = [0.0192 -0.0185 -0.0185 0.0192];
A = [1.0000 -2.8870 2.7997 -0.9113];
figure(2)
zplane(B,A);

%uloha cislo 4 -------------------
%pre vypocet kmitoctovej charakteristiky filtra z ulohy 3 pouzijem freqz()
task4 = abs(freqz(B, A, Fs/2));
figure(3)
plot(linspace(0, Fs/2, Fs/2), task4);
xlabel("f[Hz]");
ylabel("|H(f)|");

%uloha cislo 5 -------------------
%prvotny signal prefiltrujem dannym filtrom z ulohy 3
ssn = filter(B, A, sn);

%uloha cislo 6 -------------------
%filtrovany signal posuniem, prevediem na binarne symboly (rovnako ako pri ulohe 1)
%nasledne vykreslim povodny, posunuty signal a jeho binarne symboly
ssnshifted = ssn(18:end);
eigthbit_ssn = ssnshifted(8:16:end);
for i = 1:1999
  if eigthbit_ssn(i) > 0
    ssnshifted_binary_symbols(i) = 1;
  else
    ssnshifted_binary_symbols(i) = 0;
  endif
endfor

figure(4)
plot(sn(1:Fs/200))
hold on
plot(ssn(1:Fs/200))
plot(ssnshifted(1:Fs/200))
stem((1:20)*4, ssnshifted_binary_symbols(1:20))
hold off

%uloha cislo 7 -------------------
%binarne symboly signalov porovnam a vypocitam chybovost
mistakes = 0;
for i = 1:1999
  if ( sn_binary_symbols(i) != ssnshifted_binary_symbols(i) )
    mistakes = mistakes + 1;
  endif
endfor

error_rate = (mistakes/1999)*100;

%uloha cislo 8 -------------------
%spektra pomocou Fourierovej transformacie vypocitam funkciou fft()
sn_fft = abs(fft(sn)(1:Fs/2));
ssn_fft = abs(fft(ssn)(1:Fs/2));

figure(5)
plot(sn_fft);
xlabel("Hz");
figure(6)
plot(ssn_fft);
xlabel("Hz");

%uloha cislo 9 -------------------
%odhad funkcie a nasledne potrebne hodnoty na vypocet integralu poskytne funkcia hist()
[height, loc] = hist(sn);
height = height/(32000*(loc(2) - loc(1)));

figure(7)
plot(loc, hist(sn)/samples);

integral = 0;
for idx = 1:length(height)
  integral = integral + height(idx)*(loc(2) - loc(1));
endfor

%uloha cislo 10 -------------------
%vypocet korelacnych koeficientov - funkcia xcorr()
R = xcorr(sn, 50, 'biased');

figure(8)
plot((1:100) - 50, R(1:100));

%uloha cislo 11 -------------------
%R[0] = R[51], na zaklade toho posuniem indexy
r0 = R(51);
r1 = R(52);
r16 = R(67);

%uloha cislo 12 -------------------
%casovy odhad sdruzenej funkcie hustoty rozdelenia pravdepodobnosti
num = 50;
matrix = zeros(num, num);

len = length(sn);
histgr = linspace(min(sn), max(sn), 50);

[nothing, idx1] = min(abs(repmat(sn(:)', num, 1) - repmat(histgr(:), 1, len)));
idx2 = idx1(1 + 1 : len);

for i = 1 : len - 1
  mx1 = idx1(i);
  mx2 = idx2(i);
  matrix(mx1, mx2) = matrix(mx1, mx2) + 1;
endfor

surface = (histgr(2) - histgr(1)) ^ 2;
p = matrix / len / surface;

figure(9)
imagesc(histgr,histgr,p)
axis xy
colorbar

%uloha cislo 13 -------------------
%vypocet integralu pomocou sum()
sum(sum(p)) * surface

%uloha cislo 14 -------------------
%vypocet R[1] pomocou integralu (opat funckia sum())
sum(sum(repmat(histgr(:), 1, L) .* repmat(histgr(:)', L, 1) .* p)) * surface
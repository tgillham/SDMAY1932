## Author: Tim Day
## Project: SDMAY1932 Hardware Synthesizer
## Matlab algorithm for Mixer


R1 = 1000; %first Attenuator
rStep = round(10000/255,0);
RM = 1000; % Mixer Circuit

Osc1 = input('Percentage for Osc1\n');
pOsc1 = Osc1/100 *0.9;  %percentage of Osc1 scaled to high being 90 percent
resOsc1 = (pOsc1*R1)/(1-pOsc1); %attenuator adjustment
ComOsc1 = resOsc1/rStep; %divides by voltage steps
ComOsc1 = round(ComOsc1,0);
resOsc1 = ComOsc1*39 %actual resistance
voutOsc1 = resOsc1/(R1 + resOsc1); %output for later calculation

Osc2 = input('Percentage for Osc2\n');
pOsc2 = Osc2/100*0.9;
resOsc2 = (pOsc2*R1)/(1-pOsc2);
ComOsc2 = resOsc2/rStep;
ComOsc2 = round(ComOsc2,0);
resOsc2 = ComOsc2*rStep
voutOsc2 = resOsc2/(R1 + resOsc2);

noise = input('Percentage for Noise\n');
pNoise = noise/100*0.9;
resNoise = (pNoise*R1)/(1-pNoise);
ComNoise = resNoise/rStep;
ComNoise = round(ComNoise,0);
resNoise = ComNoise*rStep
voutNoise = resNoise/(R1 + resNoise);

ASK1 = input('Is something in External 1\n 1 = yes o.w = no\n');
if ASK1 == 1;
    Ext1 = input('Percentage for External 1\n');
    pExt1 = Ext1/100*0.9;
    resExt1 = (pExt1*R1)/(1-pExt1);
    ComExt1 = resExt1/rStep;
    ComExt1 = round(ComExt1,0);
    resExt1 = ComExt1*rStep
    voutExt1 = resExt1/(R1 + resExt1);
end

ASK2 = input('Is something in External 2\n 1 = yes o.w = no\n');
if ASK2 == 1;
    Ext2 = input('Percentage for External 2\n');
    pExt2 = Ext2/100*0.9;
    resExt2 = (pExt2*R1)/(1-pExt2);
    ComExt2 = resExt2/rStep;
    ComExt2 = round(ComExt2,0);
    resExt2 = ComExt2*rStep
    voutExt2 = resExt2/(R1 + resExt1);
end

ASK3 = input('Is something in External 3\n 1 = yes o.w = no\n');
if ASK3 == 1;
    Ext3 = input('Percentage for External 3\n');
    pExt3 = Ext3/100*0.9;
    resExt3 = (pExt3*R1)/(1-pExt3);
    ComExt3 = resExt3/rStep;
    ComExt3 = round(ComExt3,0);
    resExt3 = ComExt3*rStep
    voutExt3 = resExt3/(R1 + resExt3);
end


voutSum = voutOsc1 + voutOsc2 + voutNoise + voutExt1 + voutExt2 + voutExt3;
Rf = 2.5/voutSum*RM;
Rf = round(Rf,0);
ComRf = Rf/rStep;
ComRf = round(ComNoise,0);
RF = ComRf*39
Vfin = Rf/RM*voutSum 


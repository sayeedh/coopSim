function [] = createGraph(filename)
readFile = csvread(filename);
%col = [4 5 6 7 10 11 12];

%readFile = readFile(:,col);
pu = 1;
su = 2;
ns = 3;
pr = 4;
sr = 5;
chn =6;
alg =7;
avp = 8;
avs = 9;
ppr = 10;
psr = 11;
tot = 12;
k =0;
for chncond = 1: 3
    chnindices = find(readFile(:,chn) == chncond);
    temp1 = readFile(chnindices,:);
    for surate = 0.25:0.25:.25
        k =k + 1;
        sindices = find(temp1(:,sr) == surate);
        if isempty(sindices) break;end;
        temp2 = temp1(sindices,:);
        for algo = 1:3
            aindices = find(temp2(:,alg) == algo);
            avgppr(algo,k) = temp2(aindices,ppr);
            avgpsr(algo,k) = temp2(aindices,psr);
            total(algo,k) = temp2(aindices, tot);
        end
    
    end
end
bar(0.25:0.25:0.25,total);
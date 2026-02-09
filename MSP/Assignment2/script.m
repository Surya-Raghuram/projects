clc;
clear;

fid = fopen('1MBN.pdb','r');

coords = [];
rtype = [];

hydrophobic = {'ALA','VAL','LEU','ILE','MET','PHE','TRP','PRO'};
hydrophilic = {'ARG','LYS','ASP','GLU','ASN','GLN','HIS','SER','THR','TYR','CYS'};

while ~feof(fid)
    line = fgetl(fid);
    if length(line) > 54 && strcmp(line(1:4),'ATOM')
        atom = strtrim(line(13:16));
        if strcmp(atom,'CA')
            res = strtrim(line(18:20));
            x = str2double(line(31:38));
            y = str2double(line(39:46));
            z = str2double(line(47:54));

            coords = [coords; x y z];

            if ismember(res, hydrophobic)
                rtype = [rtype; 1];
            elseif ismember(res, hydrophilic)
                rtype = [rtype; 2];
            else
                rtype = [rtype; 0];
            end
        end
    end
end
fclose(fid);

center = mean(coords,1);

dist = sqrt(sum((coords - center).^2,2));
r_max = max(dist);

radii = 5:2:(r_max + 2);

denh = zeros(length(radii),1);
denp = zeros(length(radii),1);

for i = 1:length(radii)
    r = radii(i);
    inside = dist <= r;

    hcount = sum(rtype(inside) == 1);
    pcount = sum(rtype(inside) == 2);

    vol = (4/3) * pi * r^3;

    denh(i) = hcount / vol;
    denp(i) = pcount / vol;
end

figure;
plot(radii, denh, '-o', 'LineWidth', 1.5); hold on;
plot(radii, denp, '-s', 'LineWidth', 1.5);
xlabel('Radius (Å)');
ylabel('Density');
legend('Hydrophobic','Hydrophilic');
title('Cumulative Residue Density');
grid on;

saveas(gcf,'cumulative_density.png');


denh_shell = zeros(length(radii),1);
denp_shell = zeros(length(radii),1);

prev_r = 0;

for i = 1:length(radii)
    r = radii(i);
    shell = dist > prev_r & dist <= r;

    hcount = sum(rtype(shell) == 1);
    pcount = sum(rtype(shell) == 2);

    vol = (4/3) * pi * (r^3 - prev_r^3);

    denh_shell(i) = hcount / vol;
    denp_shell(i) = pcount / vol;

    prev_r = r;
end

figure;
plot(radii, denh_shell, '-o', 'LineWidth', 1.5); hold on;
plot(radii, denp_shell, '-s', 'LineWidth', 1.5);
xlabel('Radius (Å)');
ylabel('Shell Density');
legend('Hydrophobic','Hydrophilic');
title('Shell-Based Residue Density');
grid on;

saveas(gcf,'shell_density.png');


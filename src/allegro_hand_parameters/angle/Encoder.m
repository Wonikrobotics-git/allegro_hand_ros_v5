clear all;
%% ENCODER CHECK


samplingtime = 0.002;
data= load('~/test_ws/allegro_hand_gui_test/src/allegro_hand_parameters/angle/data.txt'); %%Please modify it according to the your actual data path.

[r,c]=size(data);
t=1:r;
t=t*samplingtime;
dt= samplingtime:samplingtime:r*samplingtime;
for i = 1:16
encoder(:,i) = data(:,i);
encoder_degree(:,i) = data(:,i) * 180/pi;
torque(:,i) = data(:,i+16);

end

%%MOTOR ENCODER(radian)

figure('name','[rad]angle_compare')
hold on
encoder_indices = [
    1,  5,  9, 13;
    2,  6, 10, 14;
    3,  7, 11, 15;
    4,  8, 12, 16
];

titles = {'INDEX', 'MIDDLE', 'RING', 'THUMB'};
row_labels = {'first joint(yaw) [rad]', 'second joint [rad]', 'third joint [rad]', 'fourth joint [rad]'};

for row = 1:4
    for col = 1:4
        subplot_idx = (row - 1) * 4 + col;
        subplot(4, 4, subplot_idx);
        hold on;
        plot(t, encoder(:, encoder_indices(row, col)), 'LineWidth', 2);
        xlabel('[sec]');
        if col == 1
            ylabel(row_labels{row});
        end
        if row == 1
            title(titles{col});
        end
        set(gca(), 'FontSize', 30);
    end
end


%%MOTOR ENCODER(degree)

figure('name','[deg]angle_compare')
hold on
encoder_indices = [
    1,  5,  9, 13;
    2,  6, 10, 14;
    3,  7, 11, 15;
    4,  8, 12, 16
];

titles = {'INDEX', 'MIDDLE', 'RING', 'THUMB'};
row_labels = {'first joint(yaw) [deg]', 'second joint [deg]', 'third joint [deg]', 'fourth joint [deg]'};

for row = 1:4
    for col = 1:4
        subplot_idx = (row - 1) * 4 + col;
        subplot(4, 4, subplot_idx);
        hold on;
        plot(t, encoder_degree(:, encoder_indices(row, col)), 'LineWidth', 2);
        xlabel('[sec]');
        if col == 1
            ylabel(row_labels{row});
        end
        if row == 1
            title(titles{col});
        end
        set(gca(), 'FontSize', 30);
    end
end


%%Calculated Torque


figure('name','Torque')
hold on
torque_indices = [
    1,  5,  9, 13;
    2,  6, 10, 14;
    3,  7, 11, 15;
    4,  8, 12, 16
];

titles = {'INDEX', 'MIDDLE', 'RING', 'THUMB'};
row_labels = {'first joint(yaw) [N/m]', 'second joint [N/m]', 'third joint [N/m]', 'fourth joint [N/m]'};

for row = 1:4
    for col = 1:4
        subplot_idx = (row - 1) * 4 + col;
        subplot(4, 4, subplot_idx);
        hold on;
        plot(t, torque(:, torque_indices(row, col)), 'LineWidth', 2);
        xlabel('[sec]');
        if col == 1
            ylabel(row_labels{row});
        end
        if row == 1
            title(titles{col});
        end
        set(gca(), 'FontSize', 30);
    end
end

waitforbuttonpress;
%%

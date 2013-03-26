num_sequences = length(scenes);
num_noises = length(noise_stddevs);

num_nrsfm_solvers = length(solvers.nrsfm_solvers);
num_camera_solvers = length(solvers.camera_solvers);
num_nrsfm_solvers_given_cameras = length(solvers.nrsfm_solvers_given_cameras);

num_solvers = num_nrsfm_solvers + ...
    num_camera_solvers * num_nrsfm_solvers_given_cameras;

mean_errors = shiftdim(mean(shape_errors), 1);

% [num_sequences, num_noises, num_solvers]
%   -> [num_sequences, num_solvers, num_noises]
mean_errors = permute(mean_errors, [1, 3, 2]);

% Average over frames and sequences.
mean_benchmark_errors = shiftdim(mean(mean(benchmark_errors)), 2)

for i = 1:num_noises
  noise_stddev = noise_stddevs(i);
  a = 0.5;
  b = num_solvers + 0.5;

  rel_errors = mean_errors(:, :, i) / mean_benchmark_errors(i);

  figure;
  boxplot(rel_errors);
  hold on;
  plotSpread(rel_errors);
  line([a, b], [1, 1]);
  hold off;
  grid on;

  axis([a, b, 0, max(rel_errors(:)) * 1.05]);
  title(sprintf('Noise level %g', noise_stddev));
  set(gca, 'XTick', 1:num_solvers);
  %set(gca, 'XTickLabel', {solvers.name});

%  filename = sprintf('../figures/noise/%d-%g.eps', i, noise_stddev);
%  print(filename, '-depsc2');
%  unix(['epstopdf ', filename]);
%  unix(['rm ', filename]);
end

% Generates weak-perspective cameras with projections for a sequence.
%
% Parameters:
% points -- num_frames x num_points x 3

function scene = generate_scene_for_sequence(points, omega_stddev, ...
    scale_stddev)
  num_frames = size(points, 1);
  num_points = size(points, 2);

  % Angular change in each frame.
  omegas = randn(num_frames, 1);
  % Angle in each frame.
  thetas = cumsum(omegas) + rand() * 2 * pi;
  % Scale in each frame.
  scales = exp(log(scale_stddev) * randn(num_frames, 1));

  % Construct cameras.
  cameras = weak_perspective_cameras_on_plane(thetas, scales);

  % Project points.
  projections = project_points(cameras, points);

  scene = make_scene(points, cameras, projections);
end

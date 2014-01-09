%% SETUP DIRECTORIES
stl_dir = '../rails/public/stl/';
seg_dir = strcat(stl_dir, 'seg/');
out_dir = strcat(stl_dir, 'pca/');
stls = dir(stl_dir);
segs = dir(seg_dir);
stls = stls(4:end);
segs = segs(3:end);

for i=159:159%size(segs, 1)
    display(num2str(i));
    regf = dir(strcat(seg_dir, segs(i).name));
    regf = strcat(seg_dir, segs(i).name, '/', regf(3).name);
    display(strcat(stl_dir, stls(i).name));
    display(regf);
    [f,v] = stlread(strcat(stl_dir, stls(i).name));
    [status, result] = system( ['wc -l ', regf] );
    numlines = sscanf(result, '%f %*c');

    regions = zeros(numlines, 1);
    
    %% READ SEGMENTED REGIONS
    fid = fopen(regf);
    tline = fgets(fid);
    index = 1;
    while (ischar(tline) && index < numlines)
        reg = sscanf(tline, '%f');
        regions(index) = reg;
        tline = fgets(fid);
        index = index + 1;
    end
    fclose(fid);
    
    %% FOR EVERY REGION, COMPUTE PCA
    n_reg = max(regions) + 1;
    princomps = zeros(3,3,n_reg);

    for j = 1:n_reg
        faces = find(regions == j);
        vertices = f(faces);
        data = v(vertices, :);
        [coeff, eigValueDiag] = princomp(data);
        x = coeff(:, 1)';
        yz = null(x)';
        xyz = [x; yz]';
        princomps(:, :, j) = eye(3);
    end
    princomps
    %% EXPORT AS A JSON FILE
    a = struct('file', regf, 'region', princomps);
    json = savejson('data', a);
    display(strcat(out_dir, segs(i).name,'.pca'));
    fid = fopen(strcat(out_dir, segs(i).name, '.pca'), 'w');
    fprintf(fid, json);
    fclose(fid);
end

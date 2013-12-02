%% SETUP DIRECTORIES
stl_dir = '../mesh/public/stl/';
seg_dir = strcat(stl_dir, 'seg/');
out_dir = strcat(stl_dir, 'pca/');
stls = dir(stl_dir);
segs = dir(seg_dir);
stls = stls(4:end);
segs = segs(4:end);

for i=67:size(segs, 1)
    display(i);
    regf = dir(strcat(seg_dir, segs(i).name));
    regf = strcat(seg_dir, segs(i).name, '/', regf(3).name);
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
        princomps(:, :, j) = coeff;
    end
    %% EXPORT AS A JSON FILE
    a = struct('file', regf, 'region', princomps);
    json = savejson('data', a);
    fid = fopen(strcat(out_dir, segs(i).name, '.pca'), 'w');
    fprintf(fid, json);
    fclose(fid);
end

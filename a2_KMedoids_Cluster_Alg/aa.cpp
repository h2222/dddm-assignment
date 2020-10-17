        for (int i = len-1; i >= 0; i--)
        {
            bool do_iter = true;
            for (int cen_idx : cur_medoids[cen_idx_k])
            {
                if (i == cen_idx)
                {
                    do_iter = false;
                }
            }
            if (do_iter == true)
            {
                for (int j = k-1; j >= 0; j--)
                {
                    float temp_total_cost = 0.;
                    map<string, vector<int>> temp_medoids;
                    map_copy(cur_medoids, temp_medoids, key_vec);
                    temp_medoids[cen_idx_k][j] = i;
                    assment(data, temp_medoids, temp_total_cost, k, len);
                    if (total_cost > temp_total_cost)
                    {
                        total_cost = temp_total_cost;
                        map_copy(temp_medoids, best_medoids, key_vec);
                    } 
                    else if(float_to_string(total_cost) ==  float_to_string(temp_total_cost)) 
                    {
                        if (add(temp_medoids[cen_idx_k]) < add(cur_medoids[cen_idx_k]))
                        {
                            map_copy(temp_medoids, best_medoids, key_vec);
                        }
                    }
                }
            }
        }
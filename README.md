# EABL Model Based on WPC2.0

This guide provides instructions for using the EABL model based on WPC2.0. The model requires debugging TMC2 (v21) and the corresponding HM-16.20 + SCM-8.8 version of the software. Follow the steps below to use the EABL model effectively.

## Step-by-Step Instructions

### Step 1: Debug TMC2 and HM-16.20 + SCM-8.8
1. Download and set up the TMC2 v21 version.
2. Download and set up the corresponding HM-16.20 + SCM-8.8 version.

### Step 2: Replace Files in `TAppDecoder`
1. Navigate to the following directory in the HM-16.20 + SCM-8.8 source:
2. Replace all files in this folder with the corresponding files provided by the model.

### Step 3: Replace Files in `TLibDecoder`
1. Navigate to the following directory in the HM-16.20 + SCM-8.8 source:
2. Replace all files in this folder with the corresponding files provided by the model.

### Step 3: Replace Files in `TLibDecoder`
1. Navigate to the following directory in the HM-16.20 + SCM-8.8 source:
2. Replace all files in this folder with the corresponding files provided by the model.

### Step 4: Set `TAppDecoder` as the Startup Item
1. In your IDE, set `TAppDecoder` as the startup project.
2. Run the following command (adjust paths as necessary):
3. This step will generate an output file `out.txt` that contains the `TE` information.

### Step 5: Retrieve TE Information
1. Open `out.txt` and locate the TE information that is required for the next step.

### Step 6: Calculate the Final EABL Score
1. Use MATLAB to calculate the final EABL score by calling the encrypted `EABL.p` function.
2. Example usage:
```matlab
Fmos_value = EABL(TQP, GQP, TE);

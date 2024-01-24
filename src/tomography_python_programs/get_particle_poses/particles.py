from pathlib import Path

import pandas as pd
import starfile
import typer

from ._cli import cli
from .._utils.relion import relion_pipeline_job

COMMAND_NAME = 'particles'


@cli.command(name=COMMAND_NAME, no_args_is_help=True)
@relion_pipeline_job
def combine_particle_annotations(
    tilt_series_star_file: Path = typer.Option(
        ..., help='tilt-series STAR file containing tomogram'
    ),
    annotations_directory: Path = typer.Option(
        ..., help='directory containing annotations in each tomogram'
    ),
    output_directory: Path = typer.Option(
        ..., help="directory into which 'particles.star' will be written."
    )
):
    global_df = starfile.read(tilt_series_star_file)
    global_df = global_df.set_index('rlnTomoName')
    annotation_files = annotations_directory.glob('*_particles.star')
    dfs = []
    for file in annotation_files:
        df = starfile.read(file)
        tilt_series_id = '_'.join(file.name.split('_')[:-1])
        scale_factor = float(global_df.loc[tilt_series_id, 'rlnTomoTomogramBinning'])
        xyz = df[['rlnCoordinateX', 'rlnCoordinateY', 'rlnCoordinateZ']]
        xyz = xyz.to_numpy() * scale_factor
        df[['rlnCoordinateX', 'rlnCoordinateY', 'rlnCoordinateZ']] = xyz
        dfs.append(df)
    df = pd.concat(dfs)
    output_file = output_directory / 'particles.star'
    starfile.write({'particles': df}, output_file, overwrite=True)

    df2 = pd.DataFrame({'rlnTomoParticlesFile' : [output_file],
                        'rlnTomoTomogramsFile' : [tilt_series_star_file]})
    opt_file = output_directory / 'optimisation_set.star'
    starfile.write({'optimisation_set': df2}, opt_file, overwrite=True)
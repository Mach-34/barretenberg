/**
 * Builds the web version of the worker, and outputs it to the dest directory.
 */
import { resolve, dirname } from 'path';
import { fileURLToPath } from 'url';
import ResolveTypeScriptPlugin from 'resolve-typescript-plugin';
import CopyWebpackPlugin from 'copy-webpack-plugin';
import HtmlWebpackPlugin from 'html-webpack-plugin';
import webpack from 'webpack';
// import { createRequire } from 'module';

// const require = createRequire(import.meta.url);

export default {
  target: 'web',
  mode: 'production',
  entry: {
    halo2_wasm: './src/halo2_wasm/browser/worker.ts',
    simple_test: './src/examples/simple.rawtest.ts',
  },
  module: {
    rules: [
      {
        test: /\.tsx?$/,
        use: [{ loader: 'ts-loader', options: { transpileOnly: true, onlyCompileBundledFiles: true } }],
      },
    ],
  },
  output: {
    path: resolve(dirname(fileURLToPath(import.meta.url)), './dest'),
    filename: '[name].js',
  },
  plugins: [
    new HtmlWebpackPlugin({ inject: false, template: './src/index.html' }),
    new webpack.DefinePlugin({ 'process.env.NODE_DEBUG': false }),
    new webpack.ProvidePlugin({ Buffer: ['buffer', 'Buffer'] }),
    new CopyWebpackPlugin({
      patterns: [
        {
          // Point directly to the built file, not the symlink, else copy-on-change doesn't work...
          from: `src/halo2.wasm`,
          to: 'halo2.wasm',
        },
      ],
    }),
  ],
  resolve: {
    alias: {
      './node/index.js': './browser/index.js',
    },
    plugins: [new ResolveTypeScriptPlugin()],
  },
  devServer: {
    hot: false,
    client: {
      logging: 'none',
      overlay: false,
    },
    headers: {
      'Cross-Origin-Opener-Policy': 'same-origin',
      'Cross-Origin-Embedder-Policy': 'require-corp',
    },
  },
};

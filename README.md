# HTTrack Website Copier - Development Repository

## This Fork

HTTrack ( https://github.com/xroche/httrack ) development seems stalled (last release May 2017, last commit Nov, 2019).  I make no statements or guarantees on updating this or merging PRs but if they are clean I should have the time to do so.  At a minimum should give a good starting point for someone else to continue to maintain this.

There are automatic binary builds made for every commit.   If you are logged into github you are able to download the "artifacts" or zip files of the final products and source from them.  See: [Actions · mitchcapper/httrack · GitHub](https://github.com/mitchcapper/httrack/actions/workflows/continuous.yml) for the runs, click on a run and scroll to the bottom to find artifacts.

## Fork Changes

Notable changes made on this fork:

- New Visual studio solution and projects to compile easily on VS 2022
- SSL/HTTPS Proxying support,  `--httpproxy-ssl`
- JSON cookie store rather than netscape cookies.txt (uses standardized plugin cookie fields)
- More and larger cookies (up from 8) along with larger header limits (for user agents etc)
- Nuget dependencies for openssl/zlib so no external libs need to be manually added
- WinHTTrack now pulls httrack in as a sub module for better organization and easier build
- Updated versions of zlib/openssl for windows to much newer versions
- The following PR from the original GH repo have been (or will be) merged:
  - [ x ] https://github.com/xroche/httrack/pull/135
  - [ x ] https://github.com/xroche/httrack/pull/172
  - [ x ] https://github.com/xroche/httrack/pull/199
  - [ x ] https://github.com/xroche/httrack/pull/220
  - [ x ] https://github.com/xroche/httrack/pull/221
  - [ x ] https://github.com/xroche/httrack/pull/227

## About

_Copy websites to your computer (Offline browser)_

<img src="http://www.httrack.com/htsw/screenshot_w1.jpg" width="34%">

*HTTrack* is an _offline browser_ utility, allowing you to download a World Wide website from the Internet to a local directory, building recursively all directories, getting html, images, and other files from the server to your computer.

*HTTrack* arranges the original site's relative link-structure. Simply open a page of the "mirrored" website in your browser, and you can browse the site from link to link, as if you were viewing it online.

HTTrack can also update an existing mirrored site, and resume interrupted downloads. HTTrack is fully configurable, and has an integrated help system.

*WinHTTrack* is the Windows 2000/XP/Vista/Seven release of HTTrack, and *WebHTTrack* the Linux/Unix/BSD release. 

## Website

*Main Website:*
http://www.httrack.com/

## Compiling Notes

### For Linux

```sh
git clone https://github.com/xroche/httrack.git --recurse
cd httrack
./configure --prefix=$HOME/usr && make -j8 && make install
```

### For Windows

For Windows you want to not clone this repo but rather: https://github.com/mitchcapper/httrack-windows it will pull this repo in.  You can then option the solution file in VS (tested with 2022) and it should hopefully compile (needs v143 build tools for C++).  

Alternatively you can compile using MinGW/msys tools.  Note it currently sets to v143 build tools and Win10 as the minimums.  If you need to run on older systems likely adjusting those down would not cause issues.

## 3rd party Libraries
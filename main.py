# For this section of the project, I watched a tutorial by the YouTuber "Analytic with Adam"
# URL - https://www.youtube.com/watch?v=0FtcHjI5lmw to get a solid understanding on how the YouTube API requires call
# requests for comments. I did additional research in the actual GitHub repo for the Google api python client,
# URL for GitHub Repo - https://github.com/googleapis/google-api-python-client#readme
# URL for actual YouTube API Docs - https://developers.google.com/youtube/v3/docs
# I also made use of a Geeks for geeks article on writing to csv files in python
# URL - https://www.geeksforgeeks.org/writing-csv-files-in-python/#

# need to install demoji and googleapiclient in the
# via pip install demoji
# via pip install --upgrade google-api-python-client

from googleapiclient.discovery import build
import csv
import demoji
import sys


# this function finds and removes all emojis in a string, I use it because you cannot write emojis to a csv file
def clear_emojis(string):
    dem = demoji.findall(string)
    for item in dem.keys():
        string = string.replace(item, '')
    return string


# This is my personal YouTube API Key, please be careful what videos you use so that I don't run out of API tokens


def video_data(argv):
    api_key = 'AIzaSyD7RmH3pB12AhfaYUfo39gmoJT7gBpDLac'
    video_id = argv
    # empty list for storing reply
    replies = []
    # initializing statistic variables
    title = ""
    viewCount = 0
    totLikes = 0
    numComments = 0

    # creating YouTube resource object
    youtube = build('youtube', 'v3',
                    developerKey=api_key)

    # getting general video statistics,
    video_stats = youtube.videos().list(
        part="snippet,statistics",
        id=video_id
    ).execute()

    # this try-catch adds extra input validation, if input passes prev regex but isn't and actual id, it'll get caught
    # here
    try:
        if video_stats['items'][0]['kind'] != 'youtube#video':
            print("Not a correct YouTube video ID")
            return
    except IndexError:
        print("Not a YouTube video ID")
        return

    # extracting statistics from the video_stats object
    for item in video_stats['items']:
        title = item['snippet']['title']
        viewCount = item['statistics']['viewCount']
        totLikes = item['statistics']['likeCount']
        try:
            numComments = item['statistics']['commentCount']
        except(KeyError):
            print("This video does not have comments enabled")
            print(title, " ", viewCount, " ", totLikes, " ", 0)
            return

    print(title, " ", viewCount, " ", totLikes, " ", numComments)

    # retrieve youtube video comments
    video_response = youtube.commentThreads().list(
        part='snippet,replies',
        videoId=video_id,
        maxResults=100
    ).execute()

    filename = "test.csv"

    # first row of csv file
    statsRows = [[title, viewCount, totLikes, numComments, ]]

    # writing to csv file
    with open(filename, 'w') as csvfile:
        # creating a csv writer object
        csvwriter = csv.writer(csvfile)

        # writing the video stats
        csvwriter.writerows(statsRows)

        # iterate through all the comments
        while video_response:

            # extracting required info from each result object
            for item in video_response['items']:

                # Extracting comments
                comment = item['snippet']['topLevelComment']['snippet']['textDisplay']

                clear_emojis(comment)

                commentLikes = item['snippet']['topLevelComment']['snippet']['likeCount']

                # counting number of reply of comment
                replyCount = item['snippet']['totalReplyCount']

                # if reply exists
                if replyCount > 0:

                    # iterate through all reply
                    for reply in item['replies']['comments']:
                        # Extract reply
                        reply = reply['snippet']['textDisplay']
                        cleanReply = clear_emojis(reply)

                        # Store reply in list
                        replies.append(cleanReply)

                # create a list that will be written to the csv
                dataRows = [[clear_emojis(comment), commentLikes, replies, ]]

                # need a try-catch for when demoji cannot clear all the emojies in the comment
                try:
                    csvwriter = csv.writer(csvfile)

                    csvwriter.writerows(dataRows)
                except(UnicodeEncodeError):
                    # replaces the line with an emoji with a blank line
                    dataRows = []

                # clear the list of replies
                replies = []

            # Again repeat
            if 'nextPageToken' in video_response:
                video_response = youtube.commentThreads().list(
                    part='snippet,replies',
                    videoId=video_id,
                    maxResults=100,
                    pageToken=video_response['nextPageToken']
                ).execute()
            else:
                break


if __name__ == "__main__":
    # Call function with command line argument 1 which will be the YouTube video ID
    video_data(sys.argv[1])

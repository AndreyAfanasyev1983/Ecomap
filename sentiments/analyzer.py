import nltk

def load(path):
    file_content =open(path, "r")
    
    result = set()
    add_word = False
    for line in file_content:
        word = line.strip()
        if word == "":
            add_word = True
            continue
        if not add_word:
            continue
        
        result.add(word)
    
    file_content.close()
    return result

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        self.positives = load(positives)
        self.negatives = load(negatives)
        self.tokenizer = nltk.tokenize.TweetTokenizer()
        # TODO

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        score = 0
        for word in self.tokenizer.tokenize(text):
            word = word.lower()
            if word in self.positives:
                score += 1
            elif word in self.negatives:
                score -= 1
        return score
